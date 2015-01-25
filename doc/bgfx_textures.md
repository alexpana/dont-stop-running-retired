Textures
--------

1. Use the asset builder to compile the texture to .dds and also make it available at runtime

2. Load the texture using the asset loader: dsr::loadTexture(...)

3. Create a uniform for sending the texture to the shader:
    bgfx::UniformHandle u_texColor = bgfx::createUniform("u_texColor", bgfx::UniformType::Uniform1iv);

4. Set the texture by binding it to the uniform. Make sure the 1st parameter (stage) is unique. 
    bgfx::setTexture(0, u_texColor, textureColor);
    
5. Use the SAMPLER2D macro to create a sampler for use in the texture. The macro is found in bgfx_shader.sh.
    SAMPLER2D(u_texColor, 0)
    
6. Use the regular texture2D glsl functions to sample the texture.

