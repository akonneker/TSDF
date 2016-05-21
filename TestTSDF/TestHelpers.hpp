//
//  TestHelpers.hpp
//  KinFu
//
//  Created by Dave on 16/05/2016.
//  Copyright © 2016 Sindesso. All rights reserved.
//

#ifndef TestHelpers_h
#define TestHelpers_h

#include "TSDFVolume.hpp"
#include <Eigen/Dense>


#pragma mark - helpers

phd::TSDFVolume construct_volume( int vx, int vy, int vz, float px, float py, float pz, float & vw, float & vh, float & vd);
void create_sphere_in_TSDF( phd::TSDFVolume & volume, float radius );
void create_wall_in_TSDF( phd::TSDFVolume & volume, float depth );
void save_normals_as_png( std::string filename, uint16_t width, uint16_t height, Eigen::Vector3f * normals );
void save_normals_as_colour_png( std::string filename, uint16_t width, uint16_t height, Eigen::Vector3f * normals );

void save_normals_as_png( std::string filename, uint16_t width, uint16_t height, const std::deque<Eigen::Vector3f> & normals );
void save_normals_as_colour_png( std::string filename, uint16_t width, uint16_t height, const std::deque<Eigen::Vector3f> & normals );

void save_rendered_scene_as_png(std::string filename, uint16_t width, uint16_t height, Eigen::Vector3f * vertices, Eigen::Vector3f * normals, const Eigen::Vector3f & camera_position, const Eigen::Vector3f & light_source);
void save_rendered_scene_as_png(std::string filename, uint16_t width, uint16_t height, const std::deque<Eigen::Vector3f> & vertices, const std::deque<Eigen::Vector3f> & normals, const Eigen::Vector3f & camera_position, const Eigen::Vector3f & light_source);
uint16_t * read_nyu_depth_map( const std::string & file_name, uint32_t & width, uint32_t & height );
uint16_t * read_tum_depth_map( const std::string & file_name, uint32_t & width, uint32_t & height );


Eigen::Matrix4f make_y_axis_rotation( float theta, Eigen::Vector3f pos );
Eigen::Matrix4f make_x_axis_rotation( float theta, Eigen::Vector3f pos );
Eigen::Matrix4f make_z_axis_rotation( float theta, Eigen::Vector3f pos );

#endif /* TestHelpers_h */
