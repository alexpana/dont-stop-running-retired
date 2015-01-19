/*
 * Copyright 2011-2015 Branimir Karadzic. All rights reserved.
 * License: http://www.opensource.org/licenses/BSD-2-Clause
 */

#include <bx/readerwriter.h>
#include <bx/fpumath.h>

#include "bgfx_utils.h"

#include <memory>

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

namespace bgfx_utils {

	using bx::swap;

//	static std::unique_ptr<bx::FileReaderI> sFileReaderHandle;
	static bx::FileReaderI *sFileReader;

	void init() {
//		sFileReaderHandle = std::unique_ptr<bx::FileReaderI>(new bx::CrtFileReader);
		sFileReader = new bx::CrtFileReader;
	}

	static const bgfx::Memory *loadMem(bx::FileReaderI *_reader, const char *_filePath) {
		if (0 == bx::open(_reader, _filePath)) {
			uint32_t size = (uint32_t) bx::getSize(_reader);
			const bgfx::Memory *mem = bgfx::alloc(size + 1);
			bx::read(_reader, mem->data, size);
			bx::close(_reader);
			mem->data[mem->size - 1] = '\0';
			return mem;
		}

		return nullptr;
	}

	bgfx::TextureHandle loadTexture(const char *_name, uint32_t _flags, uint8_t _skip, bgfx::TextureInfo *_info) {

		const bgfx::Memory *mem = loadMem(sFileReader, _name);

		return bgfx::createTexture(mem, _flags, _skip, _info);
	}

	bgfx::ShaderHandle loadShader(const char *name, const char *suffix) {
		char filePath[256];

		strcpy(filePath, "data/shaders/");
		strcat(filePath, name);
		strcat(filePath, suffix);

		return bgfx::createShader(loadMem(sFileReader, filePath));
	}

	bgfx::ProgramHandle loadProgram(const char *shaderName) {
		bgfx::ShaderHandle vsh = loadShader(shaderName, ".vert.bin");
		bgfx::ShaderHandle fsh = loadShader(shaderName, ".frag.bin");

		return bgfx::createProgram(vsh, fsh, true);
	}

	void calcTangents(void *_vertices, uint16_t _numVertices, bgfx::VertexDecl _decl, const uint16_t *_indices, uint32_t _numIndices) {
		struct PosTexcoord {
			float m_x;
			float m_y;
			float m_z;
			float m_pad0;
			float m_u;
			float m_v;
			float m_pad1;
			float m_pad2;
		};

		float *tangents = new float[6 * _numVertices];
		memset(tangents, 0, 6 * _numVertices * sizeof(float));

		PosTexcoord v0;
		PosTexcoord v1;
		PosTexcoord v2;

		for (uint32_t ii = 0, num = _numIndices / 3; ii < num; ++ii) {
			const uint16_t *indices = &_indices[ii * 3];
			uint32_t i0 = indices[0];
			uint32_t i1 = indices[1];
			uint32_t i2 = indices[2];

			bgfx::vertexUnpack(&v0.m_x, bgfx::Attrib::Position, _decl, _vertices, i0);
			bgfx::vertexUnpack(&v0.m_u, bgfx::Attrib::TexCoord0, _decl, _vertices, i0);

			bgfx::vertexUnpack(&v1.m_x, bgfx::Attrib::Position, _decl, _vertices, i1);
			bgfx::vertexUnpack(&v1.m_u, bgfx::Attrib::TexCoord0, _decl, _vertices, i1);

			bgfx::vertexUnpack(&v2.m_x, bgfx::Attrib::Position, _decl, _vertices, i2);
			bgfx::vertexUnpack(&v2.m_u, bgfx::Attrib::TexCoord0, _decl, _vertices, i2);

			const float bax = v1.m_x - v0.m_x;
			const float bay = v1.m_y - v0.m_y;
			const float baz = v1.m_z - v0.m_z;
			const float bau = v1.m_u - v0.m_u;
			const float bav = v1.m_v - v0.m_v;

			const float cax = v2.m_x - v0.m_x;
			const float cay = v2.m_y - v0.m_y;
			const float caz = v2.m_z - v0.m_z;
			const float cau = v2.m_u - v0.m_u;
			const float cav = v2.m_v - v0.m_v;

			const float det = (bau * cav - bav * cau);
			const float invDet = 1.0f / det;

			const float tx = (bax * cav - cax * bav) * invDet;
			const float ty = (bay * cav - cay * bav) * invDet;
			const float tz = (baz * cav - caz * bav) * invDet;

			const float bx = (cax * bau - bax * cau) * invDet;
			const float by = (cay * bau - bay * cau) * invDet;
			const float bz = (caz * bau - baz * cau) * invDet;

			for (uint32_t jj = 0; jj < 3; ++jj) {
				float *tanu = &tangents[indices[jj] * 6];
				float *tanv = &tanu[3];
				tanu[0] += tx;
				tanu[1] += ty;
				tanu[2] += tz;

				tanv[0] += bx;
				tanv[1] += by;
				tanv[2] += bz;
			}
		}

		for (uint32_t ii = 0; ii < _numVertices; ++ii) {
			const float *tanu = &tangents[ii * 6];
			const float *tanv = &tangents[ii * 6 + 3];

			float normal[4];
			bgfx::vertexUnpack(normal, bgfx::Attrib::Normal, _decl, _vertices, ii);
			float ndt = bx::vec3Dot(normal, tanu);

			float nxt[3];
			bx::vec3Cross(nxt, normal, tanu);

			float tmp[3];
			tmp[0] = tanu[0] - normal[0] * ndt;
			tmp[1] = tanu[1] - normal[1] * ndt;
			tmp[2] = tanu[2] - normal[2] * ndt;

			float tangent[4];
			bx::vec3Norm(tangent, tmp);

			tangent[3] = bx::vec3Dot(nxt, tanv) < 0.0f ? -1.0f : 1.0f;
			bgfx::vertexPack(tangent, true, bgfx::Attrib::Tangent, _decl, _vertices, ii);
		}

		delete[] tangents;
	}
}
#pragma clang diagnostic pop