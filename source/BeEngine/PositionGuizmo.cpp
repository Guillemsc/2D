#include "PositionGuizmo.h"
#include "App.h"
#include "ModuleRenderer.h"
#include "TriangleRenderer.h"
#include "LineRenderer.h"
#include "QuadRenderer.h"

void PositionGuizmo::Start()
{
	AddHandler();
	AddHandler();
	AddHandler();
}


void PositionGuizmo::Render(float relative_size)
{
	GuizmoHandler* han_up_arrow = GetHandler(PositionGuizmoHandlers::UP_ARROW);
	GuizmoHandler* han_right_arrow = GetHandler(PositionGuizmoHandlers::RIGHT_ARROW);
	GuizmoHandler* han_center = GetHandler(PositionGuizmoHandlers::CENTER);

	float3 pos = float3(0, 0, 0);

	float quad_size = 30 * relative_size;

	float triangles_height = 25 * relative_size;
	float triangles_width = 14 * relative_size;

	float lines_thickness = 1.7f * relative_size;
	float lines_lenght = 80 * relative_size;

	float3 hovered_colour = float3(247.0f / 255.0f, 188.0f / 255.0f, 43.0f / 255.0f);

	float3 line_up_colour = float3::zero;
	
	if (han_up_arrow->GetHovered())
		line_up_colour = hovered_colour;
	else
		line_up_colour = float3(178.0f / 255.0f, 242.0f / 255.0f, 82.0f / 255.0f);

	float3 line_right_colour = float3::zero;
	
	if (han_right_arrow->GetHovered())
		line_right_colour = hovered_colour;
	else
		line_right_colour = float3(220.0f / 255.0f, 61.0f / 255.0f, 30.0f / 255.0f);

	float3 center_colour = float3(1, 1, 1);

	//if (han_center->GetHovered())
	//	center_colour = hovered_colour;

	float half_quad_size = quad_size * 0.5f;
	float half_triangle_height = triangles_height * 0.5f;

	float2 line_up_start = float2(pos.x, pos.y + half_quad_size);
	float2 line_up_end = float2(pos.x, pos.y + half_quad_size + lines_lenght);

	float2 line_right_start = float2(pos.x + half_quad_size, pos.y);
	float2 line_right_end = float2(pos.x + half_quad_size + lines_lenght, pos.y);

	App->renderer->line_renderer->DrawLine(float2(pos.x - half_quad_size, pos.y - half_quad_size), 
		float2(pos.x + half_quad_size, pos.y - half_quad_size), center_colour, 0.7f, lines_thickness);

	App->renderer->line_renderer->DrawLine(float2(pos.x + half_quad_size, pos.y - half_quad_size), 
		float2(pos.x + half_quad_size, pos.y + half_quad_size), center_colour, 0.7f, lines_thickness);

	App->renderer->line_renderer->DrawLine(float2(pos.x + half_quad_size, pos.y + half_quad_size), 
		float2(pos.x - half_quad_size, pos.y + half_quad_size), center_colour, 0.7f, lines_thickness);

	App->renderer->line_renderer->DrawLine(float2(pos.x - half_quad_size, pos.y + half_quad_size), 
		float2(pos.x - half_quad_size, pos.y - half_quad_size), center_colour, 0.7f, lines_thickness);

	App->renderer->line_renderer->DrawLine(line_up_start, line_up_end, line_up_colour, 1, lines_thickness);
	App->renderer->line_renderer->DrawLine(line_right_start, line_right_end, line_right_colour, 1, 1);

	App->renderer->triangle_renderer->DrawTriangle(float2(pos.x, line_up_end.y + half_triangle_height), float2(triangles_width, triangles_height), 0, line_up_colour);
	App->renderer->triangle_renderer->DrawTriangle(float2(line_right_end.x, pos.y), float2(triangles_width, triangles_height), -90, line_right_colour);

	han_up_arrow->SetTransfroms(float2(line_up_end.x, line_up_end.y - (lines_lenght * 0.5f) + half_triangle_height), float2(triangles_width, line_up_end.y - line_up_start.y + triangles_height));
	han_right_arrow->SetTransfroms(float2(line_right_end.x - (lines_lenght * 0.5f) + half_triangle_height, line_right_start.y), float2(line_right_end.x - line_right_start.x + triangles_height, triangles_width));
	han_center->SetTransfroms(float2(pos.x, pos.y), float2(quad_size, quad_size));
}

void PositionGuizmo::UpdateTransform(float4x4 & transform)
{
}
