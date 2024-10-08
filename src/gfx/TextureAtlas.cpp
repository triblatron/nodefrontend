#include "config/config.h"

#include "gfx/TextureAtlas.h"
#include "util/enums.h"
#include "gfx/Image.h"
#include "gfx/ImageSource.h"
#include "gfx/BinImageDef.h"

#include <cstring>

namespace nfe
{
	TextureAtlas::TextureAtlas(std::size_t width, std::size_t height, std::size_t numComponents)
	:
	_errod(ERR_UNKNOWN),
	_numComponents(numComponents)
	{
		if (!isPowerOfTwo(width) || !isPowerOfTwo(height))
		{
			_errod = ERR_NON_POWER_OF_TWO_DIMS;
			return;
		}

		_binImageDef = new BinImageDef(width, height, numComponents);
		_errod = ERR_OK;
	}
	
	TextureAtlas::~TextureAtlas()
	{
		delete _binImage;
	}
	
	void TextureAtlas::pack()
	{
		if (_source != nullptr)
		{
			size_t nextX{0};
			size_t nextY{0};
			size_t maxHeightInThisShelf{0};

			using ImageDefArray = std::vector<ImageDef*>;
			ImageDefArray defs;;
			while (_source->hasMore())
			{
				nfe::ImageDef* inputImage = _source->item();
				
				if (_binImageDef!=nullptr && inputImage!=nullptr)
				{
					if ( nextX + inputImage->width() <= _binImageDef->width() && nextY + inputImage->height() <= _binImageDef->height())
					{
						allocateImage(inputImage, &maxHeightInThisShelf, &nextX, &nextY);
					}
					else
					{
						// Try next shelf
						nextX = 0u;
						nextY += maxHeightInThisShelf;
						maxHeightInThisShelf = 0u;
						if ( nextX + inputImage->width() <= _binImageDef->width() && nextY + inputImage->height() <= _binImageDef->height())
						{
							allocateImage(inputImage, &maxHeightInThisShelf, &nextX, &nextY);
						}
						else
						{
							_errod = ERR_FAILED_TO_ALLOCATE;
							break;
						}
					}
					if (ok())
					{
						defs.emplace_back(inputImage);
					}
				}
				_source->nextItem();
			}

			if (ok())
			{
				_binImage = _binImageDef->createImage();
				for (auto def : defs)
				{
					Image* image = def->createImage();
					_binImage->copyFrom(def->y(), def->x(), image);
				}
			}
		}
	}
	
	void TextureAtlas::allocateImage(ImageDef* inputImage, size_t* maxHeightInThisShelf, size_t* nextX, size_t* nextY)
	{
		if (inputImage!=nullptr && maxHeightInThisShelf != nullptr && nextX != nullptr && nextY != nullptr)
		{
			// Update max height for current shelf
			if (inputImage->height() > *maxHeightInThisShelf)
			{
				*maxHeightInThisShelf = inputImage->height();
			}
			// Copy input image
			//_binImage->copyFrom(*nextY, *nextX, inputImage);
			// Update free space
			inputImage->setPos(*nextX, *nextY);
			*nextX += inputImage->width();
			if (*nextX == _binImageDef->width())
			{
				*nextX = 0u;
				*nextY += *maxHeightInThisShelf;
				*maxHeightInThisShelf = 0u;
			}
			++_numAllocations;
		}
	}
	
	const char* TextureAtlas::errorToString(Error error)
	{
		switch (error)
		{
			ENUM_NAME(ERR_UNKNOWN)
			ENUM_NAME(ERR_OK)
			ENUM_NAME(ERR_NON_POWER_OF_TWO_DIMS)
			ENUM_NAME(ERR_FAILED_TO_ALLOCATE)
		}
		
		return "<error>";
	};
	
	TextureAtlas::Error TextureAtlas::parseError(const char* str)
	{
		TEST_ENUM(ERR_UNKNOWN, str)
		TEST_ENUM(ERR_OK, str)
		TEST_ENUM(ERR_NON_POWER_OF_TWO_DIMS, str)
		TEST_ENUM(ERR_FAILED_TO_ALLOCATE, str)
		
		return ERR_UNKNOWN;
	}
}
