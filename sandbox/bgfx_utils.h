/*
 * Copyright 2011-2015 Branimir Karadzic. All rights reserved.
 * License: http://www.opensource.org/licenses/BSD-2-Clause
 */

#pragma once

#include <bgfx.h>

namespace bgfx_utils {

	void init();

	bgfx::ShaderHandle loadShader(const char *name);

	bgfx::ProgramHandle loadProgram(const char *name);

	bgfx::TextureHandle loadTexture(const char *name, uint32_t flags = BGFX_TEXTURE_NONE, uint8_t skip = 0, bgfx::TextureInfo *info = nullptr);

	void calcTangents(void *vertices, uint16_t numVertices, bgfx::VertexDecl decl, const uint16_t *indices, uint32_t numIndices);

//	struct MeshState {
//		struct Texture {
//			uint32_t m_flags;
//			bgfx::UniformHandle m_sampler;
//			bgfx::TextureHandle m_texture;
//			uint8_t m_stage;
//		};
//
//		Texture m_textures[4];
//		uint64_t m_state;
//		bgfx::ProgramHandle m_program;
//		uint8_t m_numTextures;
//		uint8_t m_viewId;
//	};
//
//	struct Mesh;
//
//	Mesh *meshLoad(const char *_filePath);
//
//	void meshUnload(Mesh *_mesh);
//
//	MeshState *meshStateCreate();
//
//	void meshStateDestroy(MeshState *_meshState);
//
//	void meshSubmit(const Mesh *_mesh, uint8_t _id, bgfx::ProgramHandle _program, const float *_mtx, uint64_t _state = BGFX_STATE_MASK);
//
//	void meshSubmit(const Mesh *_mesh, const MeshState *const *_state, uint8_t _numPasses, const float *_mtx, uint16_t _numMatrices = 1);
}
