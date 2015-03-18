#pragma once 

//***************************************************************************
#include "../Common/IRender.h"
//***************************************************************************
#include "ILImage.h"
//***************************************************************************

namespace NGTech {

	/**
	Texture class
	*/
	class GLTexture :public I_Texture 
	{
	public:
		/**
		*/
		virtual ~GLTexture();
		/**
		*/
		virtual void setWrap(Wrap _wrap);
		/**
		*/
		virtual void setFilter(Filter _filter);
		/**
		*/
		virtual void setAniso(Aniso _aniso);
		/**
		*/
		virtual void Set();
		/**
		*/
		virtual void UnSet();
		/**
		*/
		virtual void set(size_t tex_unit);
		/**
		*/
		virtual void unset(size_t tex_unit);

		/**
		*/
		virtual void beginRenderTo();
		/**
		*/
		virtual void copy(int face = -1);
		/**
		*/
		virtual void endRenderTo();
		/**
		*/
		virtual void SetMinMipLevel(unsigned int level);
		/**
		*/
		virtual void SetMaxMipLevel(unsigned int level);
		/**
		*/
	private:
		/**
		*/
		static GLTexture *create(int width, int height, int depth, Target target, Format format, void **data);
		/**
		*/
		static GLTexture *create2d(const String &path);
		/**
		*/
		static GLTexture *createCube(const String &path);
		/**
		*/
		static GLTexture *create2d(int width, int height, Format format);
		/**
		*/
		static GLTexture *create3d(int width, int height, int depth, Format format);
		/**
		*/
		static GLTexture *createCube(int width, int height, Format format);
		/**
		*/
		static GLTexture *create2d(I_ILImage *image);
		/**
		*/
		static GLTexture *create3d(I_ILImage *image);
		/**
		*/
		static GLTexture *createCube(I_ILImage **image);
		/**
		*/
		void _ActivateTarget(void **data);
		/**
		*/
		void _ActivateFormat(Format _format);
		/**
		*/
		void _ActivateWrap(Wrap _wrap);
		/**
		*/
	private:
		friend class GLFBO;
		friend class GLSystem;
	};
}