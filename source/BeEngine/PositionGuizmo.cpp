#include "PositionGuizmo.h"
#include "App.h"
#include "ModuleSceneRenderer.h"
#include "DinamicTriangleRenderer.h"
#include "LineRenderer.h"
#include "QuadRenderer.h"
#include "ModuleInput.h"

void PositionGuizmo::Start()
{
	internal_transform = float4x4::identity;

	AddHandler();
	AddHandler();
	AddHandler();
}


void PositionGuizmo::Render(float relative_size)
{
	GuizmoHandler* han_up_arrow = GetHandler(PositionGuizmoHandlers::UP_ARROW);
	GuizmoHandler* han_right_arrow = GetHandler(PositionGuizmoHandlers::RIGHT_ARROW);
	GuizmoHandler* han_center = GetHandler(PositionGuizmoHandlers::CENTER);

	float2 pos = GetInternalPos();

	App->scene_renderer->line_renderer->SetZPos(App->scene_renderer->layer_space_guizmos.GetLayerValue(100));
	App->scene_renderer->triangle_renderer->SetZPos(App->scene_renderer->layer_space_guizmos.GetLayerValue(100));

	float quad_size = 30 * relative_size;

	float triangles_height = 25 * relative_size;
	float triangles_width = 14 * relative_size;

	float lines_thickness = 1.7f * relative_size;
	float lines_lenght = 80 * relative_size;

	float3 hovered_colour = float3(247.0f / 255.0f, 188.0f / 255.0f, 43.0f / 255.0f);
	float3 disabled_colour = float3(56.0f / 255.0f, 56.0f / 255.0f, 56.0f / 255.0f);

	float3 line_up_colour = float3(63.0f / 255.0f, 211.0f / 255.0f, 115.0f / 255.0f);
	
	float3 line_right_colour = float3(220.0f / 255.0f, 61.0f / 255.0f, 30.0f / 255.0f);

	float3 center_colour = float3(1, 1, 1);

	if (han_up_arrow->GetHovered() || han_up_arrow->GetPressed() || han_center->GetPressed())
		line_up_colour = hovered_colour;
	
	if (han_right_arrow->GetHovered() || han_right_arrow->GetPressed() || han_center->GetPressed())
		line_right_colour = hovered_colour;

	if (han_center->GetHovered() || han_center->GetPressed())
		center_colour = hovered_colour;

	if (han_up_arrow->GetPressed())
	{
		center_colour = disabled_colour;
		line_right_colour = disabled_colour;
	}

	if (han_right_arrow->GetPressed())
	{
		center_colour = disabled_colour;
		line_up_colour = disabled_colour;
	}

	float half_quad_size = quad_size * 0.5f;
	float half_triangle_height = triangles_height * 0.5f;

	float2 line_up_start = float2(pos.x, pos.y + half_quad_size);
	float2 line_up_end = float2(pos.x, pos.y + half_quad_size + lines_lenght);

	float2 line_right_start = float2(pos.x + half_quad_size, pos.y);
	float2 line_right_end = float2(pos.x + half_quad_size + lines_lenght, pos.y);

	App->scene_renderer->line_renderer->DrawLine(float2(pos.x - half_quad_size, pos.y - half_quad_size),
		float2(pos.x + half_quad_size, pos.y - half_quad_size), center_colour, 1.0f, lines_thickness);

	App->scene_renderer->line_renderer->DrawLine(float2(pos.x + half_quad_size, pos.y - half_quad_size),
		float2(pos.x + half_quad_size, pos.y + half_quad_size), center_colour, 1.0f, lines_thickness);

	App->scene_renderer->line_renderer->DrawLine(float2(pos.x + half_quad_size, pos.y + half_quad_size),
		float2(pos.x - half_quad_size, pos.y + half_quad_size), center_colour, 1.0f, lines_thickness);

	App->scene_renderer->line_renderer->DrawLine(float2(pos.x - half_quad_size, pos.y + half_quad_size),
		float2(pos.x - half_quad_size, pos.y - half_quad_size), center_colour, 1.0f, lines_thickness);

	App->scene_renderer->line_renderer->DrawLine(line_up_start, line_up_end, line_up_colour, 1, lines_thickness);
	App->scene_renderer->line_renderer->DrawLine(line_right_start, line_right_end, line_right_colour, 1, lines_thickness);

	App->scene_renderer->triangle_renderer->DrawTriangle(float2(pos.x, line_up_end.y + half_triangle_height), float2(triangles_width, triangles_height), 0, line_up_colour);
	App->scene_renderer->triangle_renderer->DrawTriangle(float2(line_right_end.x, pos.y), float2(triangles_width, triangles_height), -90, line_right_colour);

	han_up_arrow->SetTransfroms(float2(line_up_end.x, line_up_end.y - (lines_lenght * 0.5f) + half_triangle_height), float2(triangles_width, line_up_end.y - line_up_start.y + triangles_height));
	han_right_arrow->SetTransfroms(float2(line_right_end.x - (lines_lenght * 0.5f) + half_triangle_height, line_right_start.y), float2(line_right_end.x - line_right_start.x + triangles_height, triangles_width));
	han_center->SetTransfroms(float2(pos.x, pos.y), float2(quad_size, quad_size));

	if (han_up_arrow->GetPressed())
	{
		AddInternalPos(float2(0, -App->input->GetMouseYMotion() * relative_size));

		edited = true;
	}

	if (han_right_arrow->GetPressed())
	{
		AddInternalPos(float2(App->input->GetMouseXMotion() * relative_size, 0));

		edited = true;
	}

	if (han_center->GetPressed())
	{
		AddInternalPos(float2(App->input->GetMouseXMotion() * relative_size, -App->input->GetMouseYMotion() * relative_size));

		edited = true;
	}

}

bool PositionGuizmo::UpdateTransform(float4x4& transform)
{
	bool ret = false;

	if (edited)
	{
		float2 pos = GetInternalPos();
		SetMatPos(transform, pos);
	}
	else
	{
		float2 pos = GetMatPos(transform);

		SetInternalPos(pos);
	}

	ret = edited;

	edited = false;

	return ret;
}


void PositionGuizmo::SetMatPos(float4x4& transform, float2 new_pos)
{
	float3 pos;
	Quat rot;
	float3 scal;

	transform.Decompose(pos, rot, scal);


	transform = float4x4::FromTRS(float3(new_pos.x, new_pos.y, 0), rot, scal);
}

float2 PositionGuizmo::GetMatPos(const float4x4 & transform)
{
	float2 ret = float2::zero;

	Quat rot;
	float3 scale;
	float3 pos;
	transform.Decompose(pos, rot, scale);

	ret = float2(pos.x, pos.y);

	return ret;
}

float2 PositionGuizmo::GetInternalPos()
{
	return GetMatPos(internal_transform);
}

void PositionGuizmo::SetInternalPos(const float2 & pos)
{
	SetMatPos(internal_transform, pos);
}

void PositionGuizmo::AddInternalPos(const float2& to_add)
{
	Quat rot = Quat::identity;
	float3 scale = float3::one;
	float3 new_to_add = float3(to_add.x, to_add.y, 0);
	float4x4 to_add_mat = float4x4::FromTRS(new_to_add, rot, scale);

	internal_transform = internal_transform * to_add_mat;
}

