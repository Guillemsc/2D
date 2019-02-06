#ifndef __MODULE_RENDERER_H__
#define __MODULE_RENDERER_H__

#include "Module.h"
#include "Globals.h"

#include "Glew\Include\glew.h"
#include "SDL/include/SDL_opengl.h"
#include "GeometryMath.h"
#include <gl/GL.h>
#include <gl/GLU.h>

class FBO;
class ShaderProgram;

class ModuleRenderer : public Module
{
public:
	ModuleRenderer();
	~ModuleRenderer();

	bool Awake();
	bool Start();
	bool PreUpdate();
	bool PostUpdate();
	bool CleanUp();

	SDL_GLContext GetSDLGLContext() const;

	void SwapWindowBuffers();

	void SetPoligonModeWireframe() const;
	void SetPoligonModePoints(float point_size = 4.0f) const;
	void SetPoligonModeFill() const;
	void SetDepthTest(const bool& set) const;
	void SetCullFace(const bool& set) const;
	void SetLightingState(const bool& set) const;
	void SetTexture2D(const bool& set) const;
	void SetColorMaterial(const bool& set) const;
	void SetAmbientLight(const bool & enabled, const float color[4]) const;
	void SetDiffuseLight(const bool & enabled, const float color[4]) const;
	void SetSpecularLight(const bool & enabled, const float color[4]) const;

	// OpenGl control -----------------------------------------
	void SetViewport(uint start_x, uint start_y, uint width, uint height);
	void GetViewport(uint& start_x, uint& start_y, uint& width, uint& height);
	void Clear(uint buffer);

	uint GenBuffer() const;
	void UnloadBuffer(uint& id);

	void BindArrayBuffer(uint id) const;
	void BindElementArrayBuffer(uint id) const;

	void RenderElement(uint num_indices) const;

	void UnbindArraybuffer() const;
	void UnbindElementArrayBuffer() const;

	void EnableState(GLenum type) const;
	void DisableState(GLenum type) const;

	void SetVertexPointer() const;
	void SetNormalsPointer() const;
	void SetTexCoordPointer();

	uint GenTexture() const;
	void BindTexture(uint id) const;
	void BindTexture(uint target, uint id) const;
	void UnbindTexture() const;
	void UnbindTexture(uint target) const;
	void DeleteTexture(uint& id) const;

	uint GenRenderBuffer() const;
	void BindRenderBuffer(uint id) const;
	void UnbindRenderBuffer() const;

	void Set2DMultisample(uint samples, uint width, uint height);
	void SetFrameBufferTexture2D(uint target, uint id);
	void RenderStorageMultisample(uint samples, uint width, uint height);

	void LoadArrayToVRAM(uint size, const float* values, GLenum type = GL_STATIC_DRAW) const;
	void LoadElementArrayToVRAM(uint size, uint* values, GLenum type = GL_STATIC_DRAW) const;
	uint LoadTextureToVRAM(uint w, uint h, GLubyte* tex_data, GLint format) const;

	void UpdateVRAMArray(uint size, const float* values, GLenum type = GL_STATIC_DRAW) const;

	void PushMatrix();
	void PopMatrix();
	void MultMatrix(float* matrix);
	// --------------------------------------------------------

	// Shaders ------------------------------------------------
	uint GenVertexArrayBuffer() const;
	void BindVertexArrayBuffer(uint id) const;
	void UnbindVertexArrayBuffer() const;

	uint GenFrameBuffer() const;
	void BindFrameBuffer(uint id) const;
	void BindFrameBuffer(uint target, uint id) const;
	void BlitFrameBuffer(uint x, uint y, uint w, uint h) const;
	void RenderFrameBuffer(uint id) const;
	void UnbindFrameBuffer() const;
	uint CheckFrameBufferStatus();
	void DeleteFrameBuffer(uint& id);

	uint CreateVertexShader(const char* source, std::string& compilation_error_msg = std::string());
	uint CreateFragmentShader(const char* source, std::string& compilation_error_msg = std::string());
	uint CreateGeometryShader(const char* source, std::string& compilation_error_msg = std::string());
	void DeleteShader(uint shader_id);

	uint GetProgramBinary(uint program_id, uint buff_size, char* buff) const;
	int GetProgramSize(uint program_id) const;
	void LoadProgramFromBinary(uint program_id, uint buff_size, const char* buff);

	void BindAttributeLocation(uint program_id, uint index, const char* name);

	GLint GetVertexAttributeArray(uint program, const char* name);
	void EnableVertexAttributeArray(uint id);
	void DisableVertexAttributeArray(uint id);
	void SetVertexAttributePointer(uint id, uint element_size, uint elements_gap, uint infogap);

	uint GetAttributesCount(uint program);
	void GetAttributesInfo(uint program, uint index, std::string& name, GLenum& type);

	void SetUniformMatrix(uint program, const char* name, const float* data);
	void SetUniformFloat(uint program, const char* name, float data);
	void SetUniformInt(uint program, const char* name, int data);
	void SetUniformBool(uint program, const char* name, bool data);
	void SetUniformVec3(uint program, const char* name, const float3& data);
	void SetUniformVec4(uint program, const char* name, const float4& data);

	uint GetUniformsCount(uint program);
	void GetUniformInfo(uint program, uint index, std::string& name, GLenum& type);

	uint CreateShaderProgram();
	void UseShaderProgram(uint id);
	void AttachShaderToProgram(uint program_id, uint shader_id);
	bool LinkProgram(uint program_id, std::string& link_error = std::string());
	void DeleteProgram(uint program_id);
	// --------------------------------------------------------

private:
	SDL_GLContext context;
};

#endif // !__MODULE_RENDERER_H__