// =====================================================================================
//
//       Filename:  PY_PF_Chart.cpp
//
//    Description:  Wrapper file to provide Python access to PF_Chart 
//
//        Version:  1.0
//        Created:  07/21/2023 01:48:09 PM
//       Revision:  none
//       Compiler:  g++
//
//         Author:  David P. Riedel (), driedel@cox.net
//   Organization:  
//
// =====================================================================================
//

#include <decimal.hh>

using decimal::Decimal;

#include <pybind11/pybind11.h>

namespace py = pybind11;

#include "PF_Chart.h"

PYBIND11_MODULE(PY_PF_Chart, m) {
    decimal::context_template = decimal::IEEEContext(decimal::DECIMAL64);
    decimal::context_template.round(decimal::ROUND_HALF_UP);
    decimal::context = decimal::context_template;

    py::class_<PF_Chart>(m, "PY_PF_Chart")
        .def(py::init())
        .def("empty", &PF_Chart::empty) ;
}


