#include "Component.h"

Component::Component(Renderer * renderer) : Entity(renderer) {
	type: ComponentsType::NullComponentType;
}