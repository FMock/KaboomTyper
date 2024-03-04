#include "SpriteFactory.h"
#include "SpriteType.h"


namespace GameEngine
{
	class ConcreteSpriteFactory : public SpriteFactory
	{
	public:
		virtual Sprite* GetSprite(SpriteType type) override;
	};
}
