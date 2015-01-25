#include "bgfx_utils.h"
#include <memory>

#include <SDL.h>
#include <bx/readerwriter.h>
#include <bx/fpumath.h>
#include <bgfxplatform.h>

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

namespace dsr {

	using bx::FileReaderI;

	static std::unique_ptr<FileReaderI> sFileReaderHandle = std::unique_ptr<FileReaderI>(new bx::CrtFileReader);

    void initBgfx(uint16_t width, uint16_t height, const std::string &windowName) {
        SDL_Init(SDL_INIT_VIDEO);
        SDL_Window *wnd = SDL_CreateWindow(
                windowName.c_str(),
                SDL_WINDOWPOS_CENTERED,
                SDL_WINDOWPOS_CENTERED,
                width, height,
                SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

        bgfx::sdlSetWindow(wnd);
        bgfx::init();
        bgfx::reset(width, height, BGFX_RESET_VSYNC);

        bgfx::setDebug(BGFX_DEBUG_TEXT);

        bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x313233ff, 1.0f, 0);

        F32 view[16];
        bx::mtxIdentity(view);

        F32 proj[16];
        bx::mtxOrtho(proj, 0, width, height, 0, 1, 100);

        bgfx::setViewTransform(0, view, proj);

        // Set view 0 default viewport.
        bgfx::setViewRect(0, 0, 0, width, height);
    }

    static const bgfx::Memory *loadMem(bx::FileReaderI *_reader, const char *_filePath) {
		if (0 == bx::open(_reader, _filePath)) {
			U32 size = (U32) bx::getSize(_reader);
			const bgfx::Memory *mem = bgfx::alloc(size + 1);
			bx::read(_reader, mem->data, size);
			bx::close(_reader);
			mem->data[mem->size - 1] = '\0';
			return mem;
		}

		return nullptr;
	}

	bgfx::TextureHandle loadTexture(const char *_name, U32 _flags, U8 _skip, bgfx::TextureInfo *_info) {

		const bgfx::Memory *mem = loadMem(sFileReaderHandle.get(), _name);

		return bgfx::createTexture(mem, _flags, _skip, _info);
	}

	bgfx::ShaderHandle loadShader(const char *name, const char *suffix) {
		char filePath[256];

		strcpy(filePath, "data/shaders/");
		strcat(filePath, name);
		strcat(filePath, suffix);

		return bgfx::createShader(loadMem(sFileReaderHandle.get(), filePath));
	}

	bgfx::ProgramHandle loadProgram(const char *shaderName) {
		bgfx::ShaderHandle vsh = loadShader(shaderName, ".vert.bin");
		bgfx::ShaderHandle fsh = loadShader(shaderName, ".frag.bin");

		return bgfx::createProgram(vsh, fsh, true);
	}

	void calcTangents(void *_vertices, U16 _numVertices, bgfx::VertexDecl _decl, const U16 *_indices, U32 _numIndices) {
		struct PosTexcoord {
			F32 m_x;
			F32 m_y;
			F32 m_z;
			F32 m_pad0;
			F32 m_u;
			F32 m_v;
			F32 m_pad1;
			F32 m_pad2;
		};

		F32 *tangents = new F32[6 * _numVertices];
		memset(tangents, 0, 6 * _numVertices * sizeof(F32));

		PosTexcoord v0;
		PosTexcoord v1;
		PosTexcoord v2;

		for (U32 ii = 0, num = _numIndices / 3; ii < num; ++ii) {
			const uint16_t *indices = &_indices[ii * 3];
			U32 i0 = indices[0];
			U32 i1 = indices[1];
			U32 i2 = indices[2];

			bgfx::vertexUnpack(&v0.m_x, bgfx::Attrib::Position, _decl, _vertices, i0);
			bgfx::vertexUnpack(&v0.m_u, bgfx::Attrib::TexCoord0, _decl, _vertices, i0);

			bgfx::vertexUnpack(&v1.m_x, bgfx::Attrib::Position, _decl, _vertices, i1);
			bgfx::vertexUnpack(&v1.m_u, bgfx::Attrib::TexCoord0, _decl, _vertices, i1);

			bgfx::vertexUnpack(&v2.m_x, bgfx::Attrib::Position, _decl, _vertices, i2);
			bgfx::vertexUnpack(&v2.m_u, bgfx::Attrib::TexCoord0, _decl, _vertices, i2);

			const F32 bax = v1.m_x - v0.m_x;
			const F32 bay = v1.m_y - v0.m_y;
			const F32 baz = v1.m_z - v0.m_z;
			const F32 bau = v1.m_u - v0.m_u;
			const F32 bav = v1.m_v - v0.m_v;

			const F32 cax = v2.m_x - v0.m_x;
			const F32 cay = v2.m_y - v0.m_y;
			const F32 caz = v2.m_z - v0.m_z;
			const F32 cau = v2.m_u - v0.m_u;
			const F32 cav = v2.m_v - v0.m_v;

			const F32 det = (bau * cav - bav * cau);
			const F32 invDet = 1.0f / det;

			const F32 tx = (bax * cav - cax * bav) * invDet;
			const F32 ty = (bay * cav - cay * bav) * invDet;
			const F32 tz = (baz * cav - caz * bav) * invDet;

			const F32 bx = (cax * bau - bax * cau) * invDet;
			const F32 by = (cay * bau - bay * cau) * invDet;
			const F32 bz = (caz * bau - baz * cau) * invDet;

			for (U32 jj = 0; jj < 3; ++jj) {
				F32 *tanu = &tangents[indices[jj] * 6];
				F32 *tanv = &tanu[3];
				tanu[0] += tx;
				tanu[1] += ty;
				tanu[2] += tz;

				tanv[0] += bx;
				tanv[1] += by;
				tanv[2] += bz;
			}
		}

		for (U32 ii = 0; ii < _numVertices; ++ii) {
			const F32 *tanu = &tangents[ii * 6];
			const F32 *tanv = &tangents[ii * 6 + 3];

			F32 normal[4];
			bgfx::vertexUnpack(normal, bgfx::Attrib::Normal, _decl, _vertices, ii);
			F32 ndt = bx::vec3Dot(normal, tanu);

			F32 nxt[3];
			bx::vec3Cross(nxt, normal, tanu);

			F32 tmp[3];
			tmp[0] = tanu[0] - normal[0] * ndt;
			tmp[1] = tanu[1] - normal[1] * ndt;
			tmp[2] = tanu[2] - normal[2] * ndt;

			F32 tangent[4];
			bx::vec3Norm(tangent, tmp);

			tangent[3] = bx::vec3Dot(nxt, tanv) < 0.0f ? -1.0f : 1.0f;
			bgfx::vertexPack(tangent, true, bgfx::Attrib::Tangent, _decl, _vertices, ii);
		}

		delete[] tangents;
	}
}
#pragma clang diagnostic pop