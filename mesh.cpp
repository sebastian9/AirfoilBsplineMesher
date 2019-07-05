/*----------------------------------------------------------------------------------------------------------------------

        2D Airfoil mesher for Gmsh.

        Usage:

                mesh.exe bl_thickness hwall_n ratio hfar airfoil_list

        The first line is reserved for the name. The curve generated is a
        closed B-spline.  Points should be in a standard Selig format airfoil
        coordinate file.

        Usage example:

                mesh.exe 0.05 0.000001 1.3 1.5 "CH10-(smoothed).dat" e423.dat FX-84-W-150.dat S1223.dat

        Refer to http://gmsh.info/doc/texinfo/gmsh.html for documentation on Gmsh

        Refer to // https://github.com/QianZheng/BSplineFitting/ for the spline fitting code

        License: MIT License Copyright 2019 Sebastian Lopez Sanchez http://github.com/sebastian9

----------------------------------------------------------------------------------------------------------------------*/

// #define _WIN32_WINNT 0x0500 // For console resizing
// #include <windows.h>

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cmath>

/* Spline Fitting Code */
#include "spline_curve_fitting.h" // https://github.com/QianZheng/BSplineFitting/
#include "read_write_asc.h" // https://github.com/QianZheng/BSplineFitting/

using namespace std;

int main(int argc, char *argv[])
{
  /* Resize Console to show presentation */
  // HWND console = GetConsoleWindow();
  // RECT r;
  // GetWindowRect(console, &r);

  /* Presentation */
  if (!argv[1] || argc < 5) {
    // MoveWindow(window_handle, x, y, width, height, redraw_window);
    // MoveWindow(console, r.left, r.top, 950, 450, TRUE);
    cout << "\n-----------------------------------------------------------------------------------------------------------------\n" << endl;
    cout << "\t2D Airfoil mesher for Gmsh." << endl;
    cout << "\n\tUsage: \n\n\t\tmesh.exe bl_thickness hwall_n ratio hfar airfoil_list\n" << endl;
    cout << "\tThe first line is reserved for the name. The curve generated is a" << endl;
    cout << "\tclosed B-spline.  Points should be in a standard Selig format airfoil" << endl;
    cout << "\tcoordinate file." << endl;
    cout << "\n\tUsage example: \n\n\t\tmesh.exe 0.05 0.000001 1.3 1.5 \"CH10-(smoothed).dat\" e423.dat FX-84-W-150.dat S1223.dat\n" << endl;
    cout << "\tRefer to http://gmsh.info/doc/texinfo/gmsh.html for documentation on Gmsh" << endl;
    cout << "\tRefer to // https://github.com/QianZheng/BSplineFitting/ for the spline fitting code" << endl;
    cout << "\n\tLicense: MIT License Copyright 2019 Sebastian Lopez Sanchez http://github.com/sebastian9" << endl;
    cout << "\n-----------------------------------------------------------------------------------------------------------------" << endl;
    return 0;
  } else {
    // MoveWindow(window_handle, x, y, width, height, redraw_window);
    // MoveWindow(console, r.left, r.top, 950, 640, TRUE);
    cout << "\n-----------------------------------------------------------------------------------------------------------------\n" << endl;
    cout << "\t2D Airfoil mesher for Gmsh." << endl;
    cout << "\n\tMeshing: \n\n\t\t";
    for (int arg_i=5; arg_i<argc; arg_i++)
      cout << argv[arg_i] << ", ";
    cout << endl;
    cout << "\n\tMaximal thickness of the boundary layer: \n\n\t\t" << argv[1] << endl;
    cout << "\n\tMesh Size Normal to the The Wall: \n\n\t\t" << argv[2] << endl;
    cout << "\n\tSize Ratio Between Two Successive Layers: \n\n\t\t" << argv[3] << endl;
    cout << "\n\tElement size far from the wall: \n\n\t\t" << argv[4] << endl;
    cout << "\n\tGenerator function: hwall * ratio^(dist/hwall)\n" << endl;
    cout << "\tRefer to http://gmsh.info/doc/texinfo/gmsh.html for documentation on Gmsh" << endl;
    cout << "\tRefer to // https://github.com/QianZheng/BSplineFitting/ for the spline fitting code" << endl;
    cout << "\n\tLicense: MIT License Copyright 2019 Sebastian Lopez Sanchez http://github.com/sebastian9" << endl;
    cout << "\n-----------------------------------------------------------------------------------------------------------------" << endl;
  }

  /* Boundary Layer Configuration Variables */
  float bl_thickness = atof(argv[1]);
  float bl_hwall_n = atof(argv[2]);
  float bl_ratio = atof(argv[3]);
  float bl_hfar = atof(argv[4]);

  /* Declarations */
  int point_i, curve_i, curve_loop_i, surface_i; // Current Point, Curve, Curve Loop, Surface
  string run_dir = "../run/"; // Directory where gmsh, airfoils, gmsh scripts, and su2 meshes are stored

  /* Mesh airfoils in the list */
   for (int arg_i=5; arg_i<argc; arg_i++) {

     point_i = curve_i = curve_loop_i = surface_i = 0; // Current Point, Curve, Curve Loop, Surface

     ifstream airfoil_dat(run_dir + argv[arg_i]);
     string airfoil_name;
     getline(airfoil_dat, airfoil_name); // Set airfoil name to the first line in the input file
     airfoil_name.pop_back(); // Erase return
     airfoil_dat.close(); // Close input fil

     ofstream airfoil_geo; // Declare otput file for gmsh script
     airfoil_geo.open(run_dir + airfoil_name + ".geo"); // Open output file for gmsh script
     airfoil_geo << "Geometry.LineNumbers = 1; Geometry.SurfaceNumbers = 1;" << endl; // Write configutarion instructions
     // airfoil_geo << "Geometry.PointNumbers = 1;" << endl;


     /* Spline Fitting */

     // Code taken and modified from https://github.com/QianZheng/BSplineFitting/blob/master/SplineFitting/main.cpp

     CubicBSplineCurve curve(0.002); // Creates the curve with "interval" = 0.002
   	 SplineCurveFitting scf; // Creates instance of SplineCurveFitting

   	 std::vector<Vector2d> points; // Create a vector of 2D vectors (Vector2d is a class from the Eigen Library)
   	 CReadWriteAsc::readAsc( run_dir + argv[arg_i], points ); // Read the points in current file

     // Modified parameters from original code to get the desired behaviour
   	 scf.apply(points, curve, 10, 100, 0.0002, 0.0005, 0.0001, SPHERE_INIT); // Apply Bspline fit to the points in argv[arg_i] using the CubicBSplineCurve instance

     //  CReadWriteAsc::writeAsc( airfoil_name + "_controls.dat", curve.getControls()); // Output file with control points
     //  CReadWriteAsc::writeAsc( airfoil_name + "_spline.dat", curve.getSamples() ); // Output file with Bspline Sample Points

     std::vector<Vector2d> control_points = curve.getControls();

     /* Airfoil Points Creation in Gmsh File */
     for (int i =  0; i != control_points.size(); i++)
      airfoil_geo << "Point(" << ++point_i << ") = {" << control_points[i].x() << "," << control_points[i].y() << ",0};" << endl;

     /* Airfoil Curve Creation */
     airfoil_geo << "BSpline(" << ++curve_i << ") = {" << point_i << ":1," << point_i << "};" << endl;
     airfoil_geo << "Transfinite Curve {" << curve_i << "} = 200 Using Bump 1.01;" << endl;
     airfoil_geo << "Curve Loop(" << ++curve_loop_i << ") = {" << curve_i << "};" << endl;

     /* Boundary Layer */
     airfoil_geo << "Field[1] = BoundaryLayer;" << endl;
     airfoil_geo << "Field[1].thickness = " << bl_thickness << ";" << endl;
     airfoil_geo << "Field[1].hwall_n = " << bl_hwall_n << ";" << endl;
     airfoil_geo << "Field[1].ratio = " << bl_ratio << ";" << endl;
     airfoil_geo << "Field[1].hfar = " << bl_hfar << ";" << endl;
     airfoil_geo << "Field[1].Quads = 1;" << endl;
     airfoil_geo << "Field[1].EdgesList = {1,2,3,4};" << endl;
     // airfoil_geo << "Field[1].FanNodesList = {" << point_i << "};" << endl;
     airfoil_geo << "BoundaryLayer Field = 1;" << endl;

     /* Control Volume */
     airfoil_geo << "Point(" << point_i +1 << ") = {0.5,100,0};" << endl;
     airfoil_geo << "Point(" << point_i +2 << ") = {0.5,0,0};" << endl;
     airfoil_geo << "Point(" << point_i +3 << ") = {0.5,-100,0};" << endl;
     airfoil_geo << "Circle(" << ++curve_i << ") = {" << point_i +1 << "," << point_i +2 << "," << point_i +3 << "};" << endl;
     airfoil_geo << "Circle(" << ++curve_i << ") = {" << point_i +3 << "," << point_i +2 << "," << point_i +1 << "};" << endl;
     airfoil_geo << "Curve Loop(" << ++curve_loop_i << ") = {" << curve_i-1 << "," << curve_i << "};" << endl;

     /* Surface Creation */
     airfoil_geo << "Plane Surface(" << ++surface_i << ") = {" << curve_loop_i << "," << curve_loop_i-1 << "};" << endl;

     /* Boundary Conditions */
     airfoil_geo << "Physical Curve(\"airfoil\") = {1, 2, 3, 4};" << endl;
     airfoil_geo << "Physical Curve(\"farfield\") = {5, 6};" << endl;
     airfoil_geo << "Physical Surface(\"fluid\") = {1};" << endl;

     /* Mesh */
     airfoil_geo << "SetOrder 2;" << endl; // Improves agreement of the bspline and the mesh
	   airfoil_geo << "Mesh.Format = 42;" << endl; // SU2 Format
     airfoil_geo << "Mesh 2;" << endl;
     airfoil_geo << "Save \"" << run_dir << airfoil_name << ".su2\";" << endl; // Save 1D Mesh

     airfoil_geo.close();
     system((run_dir + "gmsh.exe \"" + run_dir + airfoil_name + ".geo\"").c_str()); // Execute Gmsh with input script from ./build directory
   }
   return 0;
}
/*

License:

Copyright 2019 Sebastián López Sánchez

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the
Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/
