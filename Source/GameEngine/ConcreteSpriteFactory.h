#include "SpriteFactory.h"
#include "SpriteType.h"
#include <memory>


namespace GameEngine
{
	class ConcreteSpriteFactory : public SpriteFactory
	{
	public:
		virtual std::unique_ptr<Sprite> CreateSprite(SpriteType type) override;
	};
}
