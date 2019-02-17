#pragma once

#include "Geometry.h"
#include "linalg.h"
#include <vector>

/*!
	A single contour.
	positive is (optionally) used to distinguish between polygon contours and hold contours.
*/
struct Outline2d {
	Outline2d() : positive(true) {}
	std::vector<Vector2d> vertices;
	bool positive;
};

class Polygon2d : public Geometry
{
public:
	Polygon2d() : sanitized(false) {}
	size_t memsize() const override;
	BoundingBox getBoundingBox() const override;
	std::string dump() const override;
	unsigned int getDimension() const override { return 2; }
	bool isEmpty() const override;
	Geometry *copy() const override { return new Polygon2d(*this); }

	void addOutline(const Outline2d &outline) { this->theoutlines.push_back(outline); }
	class PolySet *tessellate() const;

	typedef std::vector<Outline2d> Outlines2d;
	const Outlines2d &outlines() const { return theoutlines; }

	void transform(const Transform2d &mat);
	void resize(const Vector2d &newsize, const Eigen::Matrix<bool,2,1> &autosize);

	bool isSanitized() const { return this->sanitized; }
	void setSanitized(bool s) { this->sanitized = s; }
	bool is_convex() const;
private:
	Outlines2d theoutlines;
	bool sanitized;
};

class Polygon2dIn3d : public Polygon2d
{
public:
	Polygon2dIn3d(const Polygon2dIn3d &p): Polygon2d(p), matrix(p.matrix) {}
	Polygon2dIn3d(const Polygon2d &p, const Transform3d &m): Polygon2d(p), matrix(m) {}
	Polygon2dIn3d(const Polygon2d &p): Polygon2d(p), matrix(Transform3d::Identity()) {}

	size_t memsize() const override;
	std::string dump() const override;
	Geometry *copy() const override { return new Polygon2d(*this); }
	unsigned int getDimension() const override { return 23; }

	Transform3d matrix;
};
