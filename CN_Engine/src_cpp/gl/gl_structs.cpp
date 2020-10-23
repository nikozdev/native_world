#include <cn_pch.hpp>
#include <gl/gl_structs.h>

namespace CN
{
	// Buffer Layout
	namespace GL
	{
		void BufferLayout::addAttrib(VertexAttrib& attribute)
		{
			for (auto attrib : m_attribs)
				if (attrib == &attribute)
				{
					CN_ERR("This attribute is already set");
					return;
				}
			m_attribs.push_back(&attribute);
			m_stride += attribute.count * getTypeSz(attribute.type);
		}
	}
}