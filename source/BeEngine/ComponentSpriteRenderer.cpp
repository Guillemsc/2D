#include "ComponentSpriteRenderer.h"
#include "App.h"
#include "ResourceTexture.h"
#include "ModuleRenderer.h"
#include "StaticSpriteRenderer.h"
#include "ModuleResource.h"
#include "imgui.h"
#include "ModuleEvent.h"
#include "ModuleSceneRenderer.h"

ComponentSpriteRenderer::ComponentSpriteRenderer() : GameObjectComponent("Sprite Renderer", ComponentType::COMPONENT_TYPE_SPRITE_RENDERER,
	ComponentGroup::RENDERING, true)
{
}

ComponentSpriteRenderer::~ComponentSpriteRenderer()
{
}

void ComponentSpriteRenderer::EditorDraw()
{
	ImGui::Text("Sprite:   ");

	ImGui::SameLine();

	Resource* res = resource_texture;
	if (App->resource->EditorResourceSelector(ResourceType::RESOURCE_TYPE_TEXTURE, res, resource_filter))
	{
		resource_texture = (ResourceTexture*)res;
	}

	if (resource_texture != nullptr)
	{
		float ratio = resource_texture->GetHeightWidthRatio();
		ImGui::Image((ImTextureID)resource_texture->GetTextureId(), ImVec2(100, 100 * ratio));
	}
}

void ComponentSpriteRenderer::Start()
{
	App->event->Suscribe(std::bind(&ComponentSpriteRenderer::OnEvent, this, std::placeholders::_1), EventType::RESOURCE_DESTROYED);

	App->scene_renderer->static_sprite_renderer->AddSpriteRenderer(this);
}

void ComponentSpriteRenderer::CleanUp()
{
	App->scene_renderer->static_sprite_renderer->RemoveSpriteRenderer(this);

	App->event->UnSuscribe(std::bind(&ComponentSpriteRenderer::OnEvent, this, std::placeholders::_1), EventType::RESOURCE_DESTROYED);
}

void ComponentSpriteRenderer::OnEvent(Event * ev)
{
	if (ev->GetType() == EventType::RESOURCE_DESTROYED)
	{
		EventResourceDestroyed* erd = (EventResourceDestroyed*)ev;

		if (erd->GetResource() == resource_texture)
			resource_texture = nullptr;
	}
}

void ComponentSpriteRenderer::OnChildAdded(GameObject * child)
{
}

void ComponentSpriteRenderer::OnChildRemoved(GameObject * child)
{
}

void ComponentSpriteRenderer::OnParentChanged(GameObject * new_parent)
{
}

void ComponentSpriteRenderer::SetResourceTexture(ResourceTexture* set)
{
	resource_texture = set;
}

uint ComponentSpriteRenderer::GetTextureId() const
{
	uint ret = 0;

	if (resource_texture != nullptr)
		ret = resource_texture->GetTextureId();

	return ret;
}

bool ComponentSpriteRenderer::GetHasTexture() const
{
	return resource_texture != nullptr;
}
