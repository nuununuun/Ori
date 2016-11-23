#include "CustomDrawNode.h"
#include "base/CCEventType.h"
#include "base/CCConfiguration.h"
#include "renderer/CCRenderer.h"
#include "renderer/ccGLStateCache.h"
#include "renderer/CCGLProgramState.h"
#include "renderer/CCGLProgramCache.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerCustom.h"
#include "base/CCEventDispatcher.h"
#include "2d/CCActionCatmullRom.h"
#include "platform/CCGL.h"

USING_NS_CC;

static Vec2 v2fzero(0.0f, 0.0f);

static inline Vec2 v2f(float x, float y)
{
	Vec2 ret(x, y);
	return ret;
}

static inline Vec2 v2fadd(const Vec2 &v0, const Vec2 &v1)
{
	return v2f(v0.x + v1.x, v0.y + v1.y);
}

static inline Vec2 v2fsub(const Vec2 &v0, const Vec2 &v1)
{
	return v2f(v0.x - v1.x, v0.y - v1.y);
}

static inline Vec2 v2fmult(const Vec2 &v, float s)
{
	return v2f(v.x * s, v.y * s);
}

static inline Vec2 v2fperp(const Vec2 &p0)
{
	return v2f(-p0.y, p0.x);
}

static inline Vec2 v2fneg(const Vec2 &p0)
{
	return v2f(-p0.x, -p0.y);
}

static inline float v2fdot(const Vec2 &p0, const Vec2 &p1)
{
	return  p0.x * p1.x + p0.y * p1.y;
}

static inline Vec2 v2fforangle(float _a_)
{
	return v2f(cosf(_a_), sinf(_a_));
}

static inline Vec2 v2fnormalize(const Vec2 &p)
{
	Vec2 r(p.x, p.y);
	r.normalize();
	return v2f(r.x, r.y);
}

static inline Vec2 __v2f(const Vec2 &v)
{
	//#ifdef __LP64__
	return v2f(v.x, v.y);
	// #else
	//     return * ((Vec2*) &v);
	// #endif
}

static inline Tex2F __t(const Vec2 &v)
{
	return *(Tex2F*)&v;
}

CustomDrawNode::CustomDrawNode(int lineWidth)
{
	_vao = 0;
	_vbo = 0;
	_vaoGLPoint = 0;
	_vboGLPoint = 0;
	_vaoGLLine = 0;
	_vboGLLine = 0;
	_bufferCapacity = 0;
	_bufferCount = 0;
	_buffer = nullptr;
	_bufferCapacityGLPoint = 0;
	_bufferCountGLPoint = 0;
	_bufferGLPoint = nullptr;
	_bufferCapacityGLLine = 0;
	_bufferCountGLLine = 0;
	_bufferGLLine = nullptr;
	_dirty = false;
	_dirtyGLPoint = false;
	_dirtyGLLine = false;
	_lineWidth = lineWidth;
	_defaultLineWidth = lineWidth;

	_blendFunc = BlendFunc::ALPHA_PREMULTIPLIED;
}

CustomDrawNode::~CustomDrawNode()
{
	free(_buffer);
	_buffer = nullptr;
	free(_bufferGLPoint);
	_bufferGLPoint = nullptr;
	free(_bufferGLLine);
	_bufferGLLine = nullptr;

	glDeleteBuffers(1, &_vbo);
	glDeleteBuffers(1, &_vboGLLine);
	glDeleteBuffers(1, &_vboGLPoint);
	_vbo = 0;
	_vboGLPoint = 0;
	_vboGLLine = 0;

	if (Configuration::getInstance()->supportsShareableVAO())
	{
		GL::bindVAO(0);
		glDeleteVertexArrays(1, &_vao);
		glDeleteVertexArrays(1, &_vaoGLLine);
		glDeleteVertexArrays(1, &_vaoGLPoint);
		_vao = _vaoGLLine = _vaoGLPoint = 0;
	}
}

CustomDrawNode * CustomDrawNode::create(int defaultLineWidth)
{
	CustomDrawNode* ret = new (std::nothrow) CustomDrawNode(defaultLineWidth);
	if (ret && ret->init())
	{
		ret->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(ret);
	}

	return ret;
}

void CustomDrawNode::drawPolygon(std::vector<Vec2> verts, const Color4B & fillColor, float borderWidth, const Color4B & borderColor)
{
	CCASSERT(verts.size() >= 0, "invalid count value");

	int count = verts.size();

	bool outline = (borderColor.a > 0.0f && borderWidth > 0.0f);

	auto  triangle_count = outline ? (3 * count - 2) : (count - 2);
	auto vertex_count = 3 * triangle_count;
	ensureCapacity(vertex_count);

	V2F_C4B_T2F_Triangle *triangles = (V2F_C4B_T2F_Triangle *)(_buffer + _bufferCount);
	V2F_C4B_T2F_Triangle *cursor = triangles;

	for (int i = 0; i < count - 2; i++)
	{
		V2F_C4B_T2F_Triangle tmp = {
			{ verts[0], fillColor, __t(v2fzero) },
			{ verts[i + 1], fillColor, __t(v2fzero) },
			{ verts[i + 2], fillColor, __t(v2fzero) },
		};

		*cursor++ = tmp;
	}

	if (outline)
	{
		struct ExtrudeVerts { Vec2 offset, n; };
		struct ExtrudeVerts* extrude = (struct ExtrudeVerts*)malloc(sizeof(struct ExtrudeVerts)*count);
		memset(extrude, 0, sizeof(struct ExtrudeVerts)*count);

		for (int i = 0; i < count; i++)
		{
			Vec2 v0 = __v2f(verts[(i - 1 + count) % count]);
			Vec2 v1 = __v2f(verts[i]);
			Vec2 v2 = __v2f(verts[(i + 1) % count]);

			Vec2 n1 = v2fnormalize(v2fperp(v2fsub(v1, v0)));
			Vec2 n2 = v2fnormalize(v2fperp(v2fsub(v2, v1)));

			Vec2 offset = v2fmult(v2fadd(n1, n2), 1.0f / (v2fdot(n1, n2) + 1.0f));
			struct ExtrudeVerts tmp = { offset, n2 };
			extrude[i] = tmp;
		}

		for (int i = 0; i < count; i++)
		{
			int j = (i + 1) % count;
			Vec2 v0 = __v2f(verts[i]);
			Vec2 v1 = __v2f(verts[j]);

			Vec2 n0 = extrude[i].n;

			Vec2 offset0 = extrude[i].offset;
			Vec2 offset1 = extrude[j].offset;

			Vec2 inner0 = v2fsub(v0, v2fmult(offset0, borderWidth));
			Vec2 inner1 = v2fsub(v1, v2fmult(offset1, borderWidth));
			Vec2 outer0 = v2fadd(v0, v2fmult(offset0, borderWidth));
			Vec2 outer1 = v2fadd(v1, v2fmult(offset1, borderWidth));

			V2F_C4B_T2F_Triangle tmp1 = {
				{ inner0, Color4B(borderColor), __t(v2fneg(n0)) },
				{ inner1, Color4B(borderColor), __t(v2fneg(n0)) },
				{ outer1, Color4B(borderColor), __t(n0) }
			};
			*cursor++ = tmp1;

			V2F_C4B_T2F_Triangle tmp2 = {
				{ inner0, Color4B(borderColor), __t(v2fneg(n0)) },
				{ outer0, Color4B(borderColor), __t(n0) },
				{ outer1, Color4B(borderColor), __t(n0) }
			};
			*cursor++ = tmp2;
		}

		free(extrude);
	}

	_bufferCount += vertex_count;

	_dirty = true;
}
