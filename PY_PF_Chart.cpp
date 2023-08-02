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
/* from https://stackoverflow.com/questions/70528027/how-to-check-if-a-pyobject-isinstance-of-python-decimal-decimal-in-pybind11-wi
*
*
#include <pybind11/embed.h>
#include <iostream>
#include <typeinfo>

namespace py = pybind11;

int main() {
    py::scoped_interpreter python;
    py::object Decimal = py::module_::import("decimal").attr("Decimal");
    py::object pi = Decimal("3.14159");
    if (typeid(pi) == typeid(Decimal)) {
        std::cout << "pi is an instance of Decimal\n";
    } else {
        std::cout << "pi is not an instance of Decimal\n";
    }
}

#include <pybind11/embed.h>
#include <iostream>
#include <typeinfo>

using namespace pybind11::literals; // to bring in the `_a` literal
namespace py = pybind11;

int main() {
    py::scoped_interpreter python;
    py::object Decimal = py::module_::import("decimal").attr("Decimal");
    py::object pi = Decimal("3.14159");
    if (typeid(pi) == typeid(Decimal)) {
        std::cout << "pi is an instance of Decimal with typeid\n";
    } else {
        std::cout << "pi is not an instance of Decimal\n";
    }
    py::dict d("spam"_a=py::none(), "eggs"_a=42);
    
    if (!(typeid(d) == typeid(Decimal))) {
        std::cout << "d is not an instance of Decimal with typeid\n";
    }

    if (py::isinstance(pi, Decimal)) {
        std::cout << "pi is an instance of Decimal with py::isinstance \n";
    }
    if (!py::isinstance(d, Decimal)) {
        std::cout << "d is not an instance of Decimal with py::isinstance\n";
    }
}
*
*
*/


#include "utilities.h"
#include <pybind11/pybind11.h>

namespace py = pybind11;

#include "PF_Chart.h"

PYBIND11_MODULE(PY_PF_Chart, m) {
    py::class_<PF_Column> PF_Col(m, "PY_PF_Column");

    PF_Col.def(py::init());

    py::enum_<PF_Column::Status>(PF_Col, "Status")
        .value("e_Acceped", PF_Column::Status::e_Accepted)
        .value("e_Ignored", PF_Column::Status::e_Ignored)
        .value("e_Reversal", PF_Column::Status::e_Reversal)
        .value("e_AcceptedWithSignal", PF_Column::Status::e_AcceptedWithSignal);

    py::class_<PF_Chart>(m, "PY_PF_Chart")
        .def(py::init())
        .def(py::init([] (const std::string& symbol, const std::string_view box_size, int32_t reversals){ return PF_Chart{symbol, sv2dec(box_size), reversals}; }))
        .def("empty", &PF_Chart::empty)
        .def("GetBoxSize", [](const PF_Chart& a) { return a.GetChartBoxSize().format("f"); })
        .def("GetReversals", &PF_Chart::GetReversalboxes)
        .def("GetSymbol", &PF_Chart::GetSymbol) ;
}

    // PF_Chart(std::string symbol, decimal::Decimal base_box_size, int32_t reversal_boxes,
    //         Boxes::BoxScale box_scale=Boxes::BoxScale::e_Linear,
    //         decimal::Decimal box_size_modifier=0, int64_t max_columns_for_graph=0);

