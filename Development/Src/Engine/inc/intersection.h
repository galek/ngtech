#pragma once


namespace NGTech{

	void ScreenPosToWorldRay(
		int mouseX, int mouseY,             // Mouse position, in pixels, from bottom-left corner of the window
		int screenWidth, int screenHeight,  // Window size, in pixels
		Mat4 ViewMatrix,               // Camera position and orientation
		Mat4 ProjectionMatrix,         // Camera parameters (ratio, field of view, near and far planes)
		Vec3& out_origin,              // Ouput : Origin of the ray. /!\ Starts at the near plane, so if you want the ray to start at the camera's position instead, ignore this.
		Vec3& out_direction            // Ouput : Direction, in world space, of the ray that goes "through" the mouse.
		);

	bool TestRayOBBIntersection(
		Vec3 ray_origin,        // Ray origin, in world space
		Vec3 ray_direction,     // Ray direction (NOT target position!), in world space. Must be normalize()'d.
		Vec3 aabb_min,          // Minimum X,Y,Z coords of the mesh when not transformed at all.
		Vec3 aabb_max,          // Maximum X,Y,Z coords. Often aabb_min*-1 if your mesh is centered, but it's not always the case.
		Mat4 ModelMatrix,       // Transformation applied to the mesh (which will thus be also applied to its bounding box)
		float& intersection_distance // Output : distance between ray_origin and the intersection with the OBB
		);
}