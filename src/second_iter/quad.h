#pragma once

#include <hittable.h>

class quad: public hittable {
    private:
        point3 Q;
        vec3 u;
        vec3 v;
        shared_ptr<material> mat;
        aabb bbox;
        double delta = 0.0001;
    public:
        quad(const point3& Q, const vec3& u, const vec3& v, shared_ptr<material> mat)
        : Q(Q), u(u), v(v), mat(mat)
        {
            //
        }

        virtual void set_bounding_box() {
            //Compute the bounding box of all four vertiec
            auto bbox_diagonal1 = aabb(Q, Q + u + v);
            auto bbox_diagnal2 = aabb(Q + u, Q + v);
            bbox =aabb(bbox_diagonal1, bbox_diagnal2);
        }

        aabb bounding_box() const override {return bbox;}

        bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
            return false; // TODO
        }

};