// Copyright (c) 2016 Thomas Heller
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <hpx/algorithm.hpp>
#include <hpx/chrono.hpp>
#include <hpx/init.hpp>
#include <hpx/program_options.hpp>

#include <array>
#include <hpx/algorithm.hpp>
#include <hpx/execution.hpp>
#include <hpx/include/partitioned_vector_predef.hpp>
#include <hpx/include/partitioned_vector_view.hpp>
#include <hpx/components/containers/coarray/coarray.hpp>
#include <iostream>
#include <vector>
#include <utility>
#include <numeric>




//HPX_REGISTER_COARRAY(double);
/*
typedef std::vector<double> vector_t;

HPX_REGISTER_CHANNEL_DECLARATION(vector_t)
HPX_REGISTER_CHANNEL(vector_t, stencil_communication)
*/


void bulk_function(hpx::lcos::spmd_block block, 
                   hpx::partitioned_vector<double> data, 
                   size_t global_nx, 
                   size_t global_ny, 
                   size_t ngc) {

    
    using view_2d = hpx::partitioned_vector_view<double,2>;

    //pad with barriers (ngc on both ends)
    std::size_t NX = global_nx + 2*ngc;
    std::size_t NY = global_ny + 2*ngc;

    
    view_2d v2d(block, data.begin(), data.end(), {NX, NY});
    //view_2d subview(block,&v2d(0,0),&v2d(3,3),{3,3},{NX,NY});

    if (block.this_image() == 0) {


        //v2d(0,0) = double(43.0); //why doesnt this compile?
        v2d(0,0) = std::vector<double>{43.0, 43.0}; //what does this mean?


    }


    block.sync_all();
    
}
HPX_PLAIN_ACTION(bulk_function, bulk_action);




int hpx_main() {

    size_t ngc = 1;


    size_t global_nx = 5;
    size_t global_ny = 7;

    size_t NX = global_nx + 2*ngc;
    size_t NY = global_ny + 2*ngc;

    using vec = hpx::partitioned_vector<double>;

    auto data = vec(NX * NY);

    
    std::size_t images_per_locality = 4;

    bulk_action act;
    //This is the function call
    hpx::lcos::define_spmd_block(
        "some_name", images_per_locality, std::move(act), data, global_nx, global_ny, ngc);




  return hpx::finalize();
}

int main(int argc, char *argv[]) {
  using namespace hpx::program_options;


  return hpx::init();
}
