//
//  CUSplinePather.cpp
//  Cornell University Game Library (CUGL)
//
//  This module is a factory for producing Path2 and Poly2 objects from a Spline2.
//  In previous interations, this functionality was embedded in the Spline2
//  class.  That made that class much more heavyweight than we wanted for a
//  a simple math class.  By separating this out as a factory, we allow ourselves
//  the option of moving these calculations to a worker thread if necessary.
//
//  Because math objects are intended to be on the stack, we do not provide
//  any shared pointer support in this class.
//
//  CUGL MIT License:
//      This software is provided 'as-is', without any express or implied
//      warranty.  In no event will the authors be held liable for any damages
//      arising from the use of this software.
//
//      Permission is granted to anyone to use this software for any purpose,
//      including commercial applications, and to alter it and redistribute it
//      freely, subject to the following restrictions:
//
//      1. The origin of this software must not be misrepresented; you must not
//      claim that you wrote the original software. If you use this software
//      in a product, an acknowledgment in the product documentation would be
//      appreciated but is not required.
//
//      2. Altered source versions must be plainly marked as such, and must not
//      be misrepresented as being the original software.
//
//      3. This notice may not be removed or altered from any source distribution.
//
//  Author: Walker White
//  Version: 6/22/21
//
#include <cugl/math/polygon/CUSplinePather.h>
#include <cugl/math/CUPath2.h>
#include <cugl/math/CUPoly2.h>
#include <cugl/util/CUDebug.h>
#include <iterator>

/** Tolerance to identify a point as "smooth" */
#define SMOOTH_TOLERANCE    0.0001f

using namespace cugl;

#pragma mark Calculation
/**
 * Clears all internal data, but still maintains a reference to the spline.
 *
 * Use this method when you want to reperform the approximation at a
 * different resolution.
 */
void SplinePather::reset() {
    _calculated = false;
    _pointbuff.clear();
    _parambuff.clear();
    _anchorpts.clear();
}


/**
 * Clears all internal data, including the spline data.
 *
 * When this method is called, you will need to set a new spline before
 * calling calculate.
 */
void SplinePather::clear() {
    _calculated = false;
    _spline = nullptr;
    _pointbuff.clear();
    _parambuff.clear();
    _anchorpts.clear();
}

/**
 * Performs an approximation of the current spline
 *
 * A polygon approximation is creating by recursively calling de Castlejau's
 * until we reach a stopping condition. The stopping condition is determined
 * by the {@link Criterion}.  See that enum for a description of how the
 * various stopping conditions work.  The tolerance is the value associated
 * with the condition.  For example, for condition DISTANCE, tolerance is how
 * far the point can be away from the true curve.
 *
 * The calculation uses a reference to the spline; it does not copy it.
 * Hence this method is not thread-safe.  If you are using this method in
 * a task thread, you should copy the spline first before starting the
 * calculation.
 *
 * @param  criterion    the stopping condition criterion
 * @param  tolerance    the error tolerance of the stopping condition
 */
void SplinePather::calculate() {
    reset();
    if (!_spline) { return; }
    
    size_t size = _spline->_size;
    if (!size) { return; }
    
    const Vec2* points = (_spline->_points.data());
    _pointbuff.clear();
    
    for (size_t ii = 0; ii < _spline->_size; ii++) {
        _anchorpts[_pointbuff.size()] = ii;
        generate((float)ii, points+(3*ii), points+(3*ii+1), points+(3*ii+2), points+(3*ii+3), 0);
    }
    
    // Push back last point and parameter
    _anchorpts[_pointbuff.size()] = _spline->_size;
    _pointbuff.push_back(_spline->_points[3 * _spline->_size]);
    _parambuff.push_back((float)_spline->_size);
    _closed = _spline->_closed;
    _calculated = true;
}

/**
 * Generates data via recursive use of de Castlejau's
 *
 * This method subdivides the spline at the given segment. The results
 * are put in the output buffers.
 *
 * @param  t        the parameter for the (start of) this segment
 * @param  p0       the left anchor of this segment
 * @param  p1       the left tangent of this segment
 * @param  p2       the right tangent of this segment
 * @param  p3       the right anchor of this segment
 * @param  depth    the current depth of the recursive call
 *
 * @return The number of (anchor) points generated by this recursive call.
 */
int SplinePather::generate(float t, const Vec2* p0, const Vec2* p1,
                           const Vec2* p2, const Vec2* p3, int depth) {
    // Do not go to far
    bool terminate = false;
    if (depth >= 8) {
        terminate = true;
    } else if ((*p0 == *p1) && (*p2 == *p3)) {
        terminate = true;
    } else {
        float dx = p3->x - p0->x;
        float dy = p3->y - p0->y;
        float d2 = (((p1->x - p3->x) * dy - (p1->y - p3->y) * dx));
        float d3 = (((p2->x - p3->x) * dy - (p2->y - p3->y) * dx));
        d2 = d2 > 0 ? d2 : -d2;
        d3 = d3 > 0 ? d3 : -d3;
    
        if ((d2 + d3)*(d2 + d3) < _tolerance * (dx*dx + dy*dy)) {
            terminate = true;
        }
    }
    
    // Add the first point if terminating.
    int result = 0;
    if (terminate) {
        //CULog("Terminate at %.3f [%d]\n",param,depth);
        _parambuff.push_back(t);
        _pointbuff.push_back(*p0);
        _pointbuff.push_back(*p1);
        _pointbuff.push_back(*p2);
        return 1;
    }
    
    // Cross bar
    Vec2 h  = (*p1 + *p2)*0.5f;

    // Subdivisions
    Vec2 l1 = (*p0 + *p1)*0.5f;
    Vec2 l2 = ( l1 +   h)*0.5f;
    Vec2 r2 = (*p2 + *p3)*0.5f;
    Vec2 r1 = (  h +  r2)*0.5f;
    Vec2 c =  ( l2 +  r1)*0.5f;
    
    // Recursive calls
    float s = t + 1.0f / (1 << (depth + 1));
    result =  generate(t, p0, &l1, &l2, &c, depth + 1);
    result += generate(s, &c, &r1, &r2, p3, depth + 1);
    return result;
}

#pragma mark -
#pragma mark Materialization
/**
 * Returns a new polygon approximating this spline.
 *
 * The Poly2 indices will define a path traversing the vertices of the
 * polygon.  Hence this Poly2 may be drawn as a wireframe.  The indices
 * will define a closed path if the spline is itself closed, and an open
 * path otherwise.
 *
 * The resolution of the polygon is determined by the {@link calculate()}
 * method.  See the description of that method for the various options.
 * If calculate has not been called, this method will create a polygon
 * from the control points on the original spline.
 *
 * @return a new polygon approximating this spline.
 */
Path2 SplinePather::getPath() const {
    const std::vector<Vec2>* points = getActivePoints();
    if (!points) { return Path2(); }
    
    Path2 path;
    int size = (int)points->size();

    int amt = (size-1)/3;
    size_t limit = isClosed() ? size-4 : size-1;
    path.vertices.reserve(amt);
    path.corners.reserve(amt);
    for(int ii = 0; 3*ii <= limit; ii++) {
        path.vertices.push_back(points->at(3*ii));
        for(auto it = _anchorpts.begin(); it != _anchorpts.end(); ++it) {
            if (it->first % 3 == 0 && !_spline->_smooth[it->second]) {
                path.corners.emplace(it->first/3);
            }
        }
    }
    path.closed = isClosed();
    return path;
}

/**
 * Stores vertex information approximating this spline in the buffer.
 *
 * The resolution of the path is determined by the {@link #setTolerance}
 * method.  See the description of that method for how this effects the
 * stopping condition of the calculation.
 *
 * The vertices (and indices) will be appended to the the Path2 object
 * if it is not empty. You should clear the path first if you do not
 * want to preserve the original data.
 *
 * @param buffer    The buffer to store the vertex data
 *
 * @return a reference to the buffer for chaining.
 */
Path2* SplinePather::getPath(Path2* buffer) const {
    CUAssertLog(buffer, "Destination buffer is null");
    const std::vector<Vec2>* points = getActivePoints();
    if (!points) { return buffer; }

    int size = (int)points->size();
    int amt = (size-1)/3+1;
    size_t limit = isClosed() ? size-4 : size-1;
    size_t bsize = buffer->vertices.size();
    buffer->reserve(bsize+amt);
    for(int ii = 0; 3*ii <= limit; ii++) {
        buffer->vertices.push_back(points->at(3*ii));
        for(auto it = _anchorpts.begin(); it != _anchorpts.end(); ++it) {
            if (it->first % 3 == 0 && !_spline->_smooth[it->second]) {
                buffer->corners.emplace(it->first/3+bsize);
            }
        }
    }
    buffer->closed = isClosed();
    return buffer;
}


/**
 * Returns a list of parameters for a polygon approximation
 *
 * The parameters correspond to the generating values in the spline
 * polynomial.  That is, if you evaluate the polynomial on the parameters,
 * {via {@link Bezerier.getPoint()}, you will get the points in the
 * approximating polygon.
 *
 * The resolution of the polygon is determined by the {@link calculate()}
 * method.  See the description of that method for the various options.
 * If calculate has not been called, this method will choose parameters
 * for the control points on the original spline.
 *
 * @return a list of parameters for a polygon approximation
 */
std::vector<float> SplinePather::getParameters() const {
    std::vector<float> result;
    if (_calculated) {
        result.reserve(_parambuff.size());
        result.assign(_parambuff.begin(), _parambuff.end());
    } else if (_spline) {
        result.reserve(_spline->_size+1);
        for(int ii = 0; ii <= _spline->_size; ii++) {
            result.push_back((float)ii);
        }
    }
    return result;
}

/**
 * Stores a list of parameters for the approximation in the buffer.
 *
 * The parameters correspond to the generating values in the spline
 * polynomial.  That is, if you evaluate the polynomial on the parameters,
 * {via {@link Bezerier.getPoint()}, you will get the points in the
 * approximating polygon.
 *
 * The resolution of the polygon is determined by the {@link calculate()}
 * method.  See the description of that method for the various options.
 * If calculate has not been called, this method will choose parameters
 * for the control points on the original spline.
 *
 * The parameters will be appended to the buffer vector.  You should clear
 * the buffer first if you do not want to preserve the original data.
 *
 * @param buffer    The buffer to store the parameter data
 *
 * @return the number of elements added to the buffer
 */
size_t SplinePather::getParameters(std::vector<float>& buffer) {
    if (_calculated) {
        buffer.reserve(buffer.size()+_parambuff.size());
        std::copy(_parambuff.begin(),_parambuff.end(),std::back_inserter(buffer));
        return _parambuff.size();
    } else if (_spline) {
        buffer.reserve(buffer.size()+_spline->_size);
        for(int ii = 0; ii <= _spline->_size; ii++) {
            buffer.push_back((float)ii);
        }
        return _spline->_size+1;
    }
    return 0;
}

/**
 * Returns a list of tangents for a polygon approximation
 *
 * These tangent vectors are presented in control point order.  First, we
 * have the right tangent of the first point, then the left tangent of the
 * second point, then the right, and so on.  Hence if the polygon contains
 * n points, this method will return 2(n-1) tangents.
 *
 * The resolution of the polygon is determined by the {@link calculate()}
 * method.  See the description of that method for the various options.
 * If calculate has not been called, this method will choose tangents
 * for the control points on the original spline.  This latter option
 * is useful when you want to draw a UI for the control point tangents.
 *
 * @return a list of tangents for a polygon approximation
 */
std::vector<Vec2> SplinePather::getTangents() const {
    const std::vector<Vec2>* points = getActivePoints();
    std::vector<Vec2> result;

    if (!points) { return result; }

    int size = (int)points->size();
    int amt = 2*(((size-1)/3)-1);
    result.reserve(amt);
    
    for(int ii = 0; 3*ii < size-1; ii++) {
        result.push_back(points->at(3*ii+1)-points->at(3*ii));
        result.push_back(points->at(3*ii+2)-points->at(3*ii+3));
    }
    
    // Push back the last vector.
    result.push_back(points->at(size-2)-points->at(size-1));
    return result;
}

/**
 * Stores a list of tangents for the approximation in the buffer.
 *
 * These tangent vectors are presented in control point order.  First, we
 * have the right tangent of the first point, then the left tangent of the
 * second point, then the right, and so on.  Hence if the polygon contains
 * n points, this method will return 2(n-1) tangents.
 *
 * The resolution of the polygon is determined by the {@link calculate()}
 * method.  See the description of that method for the various options.
 * If calculate has not been called, this method will choose tangents
 * for the control points on the original spline.  This latter option
 * is useful when you want to draw a UI for the control point tangents.
 *
 * The tangents will be appended to the buffer vector.  You should clear
 * the buffer first if you do not want to preserve the original data.
 *
 * @return the number of elements added to the buffer
 */
size_t SplinePather::getTangents(std::vector<Vec2>& buffer) {
    const std::vector<Vec2>* points = getActivePoints();
    if (!points) { return 0; }

    int size = (int)points->size();
    int amt = 2*(((size-1)/3)-1);
    buffer.reserve(buffer.size()+amt);
    
    for(int ii = 0; 3*ii < size-1; ii++) {
        buffer.push_back(points->at(3*ii+1)-points->at(3*ii));
        buffer.push_back(points->at(3*ii+2)-points->at(3*ii+3));
    }
    
    // Push back the last vector.
    buffer.push_back(points->at(size-2)-points->at(size-1));
    return amt;
}

/**
 * Returns a list of normals for a polygon approximation
 *
 * There is one normal per control point. If polygon contains n points,
 * this method will also return n normals. The normals are determined by the
 * right tangents.  If the spline is open, then the normal of the last point
 * is determined by its left tangent.
 *
 * The resolution of the polygon is determined by the {@link calculate()}
 * method.  See the description of that method for the various options.
 * If calculate has not been called, this method will choose normals
 * for the control points on the original spline.  This latter option
 * is useful when you want to draw a UI for the control point normals.
 *
 * @return a list of normals for a polygon approximation
 */
std::vector<Vec2> SplinePather::getNormals() const {
    const std::vector<Vec2>* points = getActivePoints();
    std::vector<Vec2> result;
    
    if (!points) { return result; }

    int size = (int)points->size();
    int amt = (size-1)/3;
    result.reserve(amt);
    
    Vec2 temp;
    for(int ii = 0; 3*ii < size-1; ii++) {
        temp = points->at(3*ii+1)-points->at(3*ii);
        temp.perp();
        result.push_back(temp);
    }
    
    // Push back the last normal.
    temp = points->at(size-1)-points->at(size-2);
    temp.perp();
    
    result.push_back(temp);
    return result;
}

/**
 * Stores a list of normals for the approximation in the buffer.
 *
 * There is one normal per control point. If polygon contains n points,
 * this method will also return n normals. The normals are determined by the
 * right tangents.  If the spline is open, then the normal of the last point
 * is determined by its left tangent.
 *
 * The resolution of the polygon is determined by the {@link calculate()}
 * method.  See the description of that method for the various options.
 * If calculate has not been called, this method will choose normals
 * for the control points on the original spline.  This latter option
 * is useful when you want to draw a UI for the control point normals.
 *
 * The normals will be appended to the buffer vector.  You should clear
 * the buffer first if you do not want to preserve the original data.
 *
 * @return the number of elements added to the buffer
 */
size_t SplinePather::getNormals(std::vector<Vec2>& buffer) {
    const std::vector<Vec2>* points = getActivePoints();

    int size = (int)points->size();
    int amt = (size-1)/3;
    buffer.reserve(buffer.size()+amt);
    
    Vec2 temp;
    for(int ii = 0; 3*ii < size-1; ii++) {
        temp = points->at(3*ii+1)-points->at(3*ii);
        temp.perp();
        buffer.push_back(temp);
    }
    
    // Push back the last normal.
    temp = points->at(size-1)-points->at(size-2);
    temp.perp();
    
    buffer.push_back(temp);
    return amt;
}

/**
 * Fills in the vertex and index data for a single handle.
 *
 * Handles are circular shapes of a given radius. This information may be
 * passed to a PolygonNode to provide a visual representation of the
 * anchor points (as seen in Adobe Illustrator).
 *
 * @param  point    The location of the handle point
 * @param  radius   the radius of each handle
 * @param  segments the number of segments in the handle "circle"
 * @param  vertices the vector storing the vertex data
 * @param  indices  the vector storing the index data
 */
static void fillHandle(const Vec2 point, float radius, int segments,
                std::vector<Vec2> vertices, std::vector<Uint32> indices) {
    // Figure out the starting vertex
    int offset = (int)vertices.size();
    
    // Add the center
    vertices.reserve(vertices.size()+segments+1);
    vertices.push_back(point);

    // Add everything else
    float coef = 2.0f * (float)M_PI / segments;
    Vec2 temp;
    for (int jj = 0; jj <= segments; jj++) {
        float rads = jj*coef;
        temp.set(0.5f * radius * cosf(rads),0.5f * radius * sinf(rads));
        vertices.push_back(temp+point);
    }
    
    indices.reserve(vertices.size()+3*segments);
    for (int jj = 0; jj < segments; jj++) {
        indices.push_back(offset);
        indices.push_back(offset+jj+1);
        indices.push_back(offset+jj+2);
    }
}


/**
 * Returns a Poly2 representing handles for the anchor points
 *
 * This method returns a collection of vertex information for handles at
 * the anchor points.  Handles are circular shapes of a given radius. This
 * information may be drawn to provide a visual representation of the
 * anchor points (as seen in Adobe Illustrator).
 *
 * The resolution of the polygon is determined by the {@link calculate()}
 * method.  See the description of that method for the various options.
 * If calculate has not been called, this method will choose anchors
 * for the control points on the original spline.  This latter option
 * is useful when you want to draw a UI for the original control points.
 *
 * @param  radius   the radius of each handle
 * @param  segments the number of segments in the handle "circle"
 *
 * @return a Poly2 representing handles for the anchor points
 */
Poly2 SplinePather::getAnchors(float radius, int segments) const {
    const std::vector<Vec2>* points = getActivePoints();
    if (!points) { return Poly2(); }

    int size = (int)points->size();
    int last = (_closed ? (size - 4)/3 : (size - 1)/3);
    
    Poly2 poly;
    for (int ii = 0; ii <= last; ii++) {
        fillHandle(points->at(3 * ii), radius, segments, poly.vertices, poly.indices);
    }
    return poly;
}

/**
 * Stores vertex information representing the anchor points in the buffer.
 *
 * This method creates a collection of vertex information for handles at
 * the anchor points.  Handles are circular shapes of a given radius. This
 * information may be drawn to provide a visual representation of the
 * anchor points (as seen in Adobe Illustrator).
 *
 * The resolution of the polygon is determined by the {@link calculate()}
 * method.  See the description of that method for the various options.
 * If calculate has not been called, this method will choose anchors
 * for the control points on the original spline.  This latter option
 * is useful when you want to draw a UI for the original control points.
 *
 * The vertices (and indices) will be appended to the the Poly2 if it is
 * not empty. You should clear the Poly2 first if you do not want to
 * preserve the original data.
 *
 * @param  buffer   The buffer to store the vertex data
 * @param  radius   The radius of each handle
 * @param  segments The number of segments in the handle "circle"
 *
 * @return a reference to the buffer for chaining.
 */
Poly2* SplinePather::getAnchors(Poly2* buffer, float radius, int segments) const {
    CUAssertLog(buffer, "Destination buffer is null");
    const std::vector<Vec2>* points = getActivePoints();
    if (!points) { return buffer; }

    int size = (int)points->size();
    int last = (_closed ? (size - 4)/3 : (size - 1)/3);
    
    for (int ii = 0; ii <= last; ii++) {
        fillHandle(points->at(3 * ii), radius, segments, buffer->vertices, buffer->indices);
    }
    return buffer;
}

/**
 * Returns a Poly2 representing handles for the tangent points
 *
 * This method returns vertex information for handles at the tangent
 * points.  Handles are circular shapes of a given radius. This information
 * may be passed to a PolygonNode to provide a visual representation of the
 * tangent points (as seen in Adobe Illustrator).
 *
 * The resolution of the polygon is determined by the {@link calculate()}
 * method.  See the description of that method for the various options.
 * If calculate has not been called, this method will choose the tangents
 * from the control points on the original spline.  This latter option
 * is useful when you want to draw a UI for the original tangent points.
 *
 * @param  radius   the radius of each handle
 * @param  segments the number of segments in the handle "circle"
 *
 * @return a Poly2 representing handles for the tangent points
 */
Poly2 SplinePather::getHandles(float radius, int segments) const {
    const std::vector<Vec2>* points = getActivePoints();
    if (!points) { return Poly2(); }

    int size = (int)points->size();

    Poly2 poly;
    for (int ii = 0; 3*ii < size; ii++) {
        fillHandle(points->at(3 * ii + 1), radius, segments, poly.vertices, poly.indices);
        fillHandle(points->at(3 * ii + 2), radius, segments, poly.vertices, poly.indices);
    }
    return poly;
}


/**
 * Stores vertex information representing tangent point handles in the buffer.
 *
 * This method creates vertex information for handles at the tangent
 * points.  Handles are circular shapes of a given radius. This information
 * may be passed to a PolygonNode to provide a visual representation of the
 * tangent points (as seen in Adobe Illustrator).
 *
 * The resolution of the polygon is determined by the {@link calculate()}
 * method.  See the description of that method for the various options.
 * If calculate has not been called, this method will choose the tangents
 * from the control points on the original spline.  This latter option
 * is useful when you want to draw a UI for the original tangent points.
 *
 * The vertices (and indices) will be appended to the the Poly2 if it is
 * not empty. You should clear the Poly2 first if you do not want to
 * preserve the original data.
 *
 * @param  buffer   The buffer to store the vertex data
 * @param  radius   the radius of each handle
 * @param  segments the number of segments in the handle "circle"
 *
 * @return a reference to the buffer for chaining.
 */
Poly2* SplinePather::getHandles(Poly2* buffer, float radius, int segments) const {
    CUAssertLog(buffer, "Destination buffer is null");
    const std::vector<Vec2>* points = getActivePoints();
    if (!points) { return buffer; }

    int size = (int)points->size();
    for (int ii = 0; 3*ii < size; ii++) {
        fillHandle(points->at(3 * ii + 1), radius, segments, buffer->vertices, buffer->indices);
        fillHandle(points->at(3 * ii + 2), radius, segments, buffer->vertices, buffer->indices);
    }
    return buffer;

}

/**
 * Returns an expanded version of this spline
 *
 * When we use de Castlejau's to approximate the spline, it produces a list
 * of control points that are geometrically equal to this spline (e.g. ignoring
 * parameterization). Instead of flattening this information to a polygon,
 * this method presents this data as a new spline.
 *
 * The resolution of the polygon is determined by the {@link calculate()}
 * method.  See the description of that method for the various options.
 * If calculate has not been called, this method will copy the original spline.
 *
 * @return an expanded version of this spline
 */
Spline2 SplinePather::getRefinement() const {
    const std::vector<Vec2>* points = getActivePoints();
    if (!points) { return Spline2(); }

    Spline2 result(*points);
    result.setClosed(isClosed());
    return result;
}

/**
 * Stores an expanded version of this spline in the given buffer.
 *
 * When we use de Castlejau's to approximate the spline, it produces a list
 * of control points that are geometrically equal to this spline (e.g. ignoring
 * parameterization). Instead of flattening this information to a polygon,
 * this method presents this data as a new spline.
 *
 * The resolution of the polygon is determined by the {@link calculate()}
 * method.  See the description of that method for the various options.
 * If calculate has not been called, this method will copy the original spline.
 *
 * The control points will be appended to the the spline if it is not
 * empty. You should clear the spline first if you do not want to preserve
 * the original data.
 *
 * @param  buffer   The buffer to store the vertex data
 *
 * @return a reference to the buffer for chaining.
 */
Spline2* SplinePather::getRefinement(Spline2* buffer) const {
    CUAssertLog(buffer, "Destination buffer is null");
    const std::vector<Vec2>* points = getActivePoints();
    if (!points) { return buffer; }

    CUAssertLog(!buffer->isClosed(), "Cannot append to closed curve");
    int size = (int)points->size();
    int amt  = (size-1)/3;
    
    int boff = (int)buffer->_points.size();
    buffer->_points.reserve(boff + size);
    std::copy(points->begin(), points->end(), std::back_inserter(buffer->_points));
    
    int soff = (int)buffer->_smooth.size();
    buffer->_smooth.resize(soff + amt + 1, true);
    buffer->_smooth[soff] = true;
    buffer->_smooth[soff+amt] = true;
    
    for (int ii = (boff == 0 ? 1 : boff); ii < boff+amt; ii++) {
        Vec2 temp0 = buffer->_points.at(3 * ii - 1) - buffer->_points.at(3 * ii);
        Vec2 temp1 = buffer->_points.at(3 * ii + 1) - buffer->_points.at(3 * ii);
        temp0.normalize();
        temp1.normalize();
        temp0 -= temp1;
        buffer->_smooth[ii] = (temp0.lengthSquared() < SMOOTH_TOLERANCE);
    }
    
    return buffer;
}
