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

#include <format>
#include <pybind11/operators.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
// #include <pybind11/options.h>

namespace py = pybind11;

#include "PF_Chart.h"
#include "utilities.h"

PYBIND11_MODULE(PY_PF_Chart, m)
{
    decimal::context_template = decimal::IEEEContext(decimal::DECIMAL64);
    decimal::context_template.round(decimal::ROUND_HALF_EVEN);
    decimal::context = decimal::context_template;

    py::class_<std::filesystem::path>(m, "Path").def(py::init<std::string>());
    py::implicitly_convertible<std::string, std::filesystem::path>();

    py::enum_<PF_SignalCategory>(m, "PF_SignalCategory")
        .value("e_Unknown", PF_SignalCategory::e_unknown)
        .value("e_PF_Buy", PF_SignalCategory::e_PF_Buy)
        .value("e_PF_Sell", PF_SignalCategory::e_PF_Sell);

    py::enum_<PF_SignalType>(m, "PF_SignalType")
        .value("e_unknown", PF_SignalType::e_unknown)
        .value("e_double_top_buy", PF_SignalType::e_double_top_buy)
        .value("e_double_bottom_sell", PF_SignalType::e_double_bottom_sell)
        .value("e_triple_top_buy", PF_SignalType::e_triple_top_buy)
        .value("e_triple_bottom_sell", PF_SignalType::e_triple_bottom_sell)
        .value("e_bullish_tt_buy", PF_SignalType::e_bullish_tt_buy)
        .value("e_bearish_tb_sell", PF_SignalType::e_bearish_tb_sell)
        .value("e_catapult_buy", PF_SignalType::e_catapult_buy)
        .value("e_catapult_sell", PF_SignalType::e_catapult_sell)
        .value("e_ttop_catapult_buy", PF_SignalType::e_ttop_catapult_buy)
        .value("e_tbottom_catapult_sell", PF_SignalType::e_tbottom_catapult_sell);

    py::enum_<PF_SignalPriority>(m, "PF_SignalPriority")
        .value("e_unknown", PF_SignalPriority::e_unknown)
        .value("e_double_top_buy", PF_SignalPriority::e_double_top_buy)
        .value("e_double_bottom_sell", PF_SignalPriority::e_double_bottom_sell)
        .value("e_triple_top_buy", PF_SignalPriority::e_triple_top_buy)
        .value("e_triple_bottom_sell", PF_SignalPriority::e_triple_bottom_sell)
        .value("e_bullish_tt_buy", PF_SignalPriority::e_bullish_tt_buy)
        .value("e_bearish_tb_sell", PF_SignalPriority::e_bearish_tb_sell)
        .value("e_catapult_buy", PF_SignalPriority::e_catapult_buy)
        .value("e_catapult_sell", PF_SignalPriority::e_catapult_sell)
        .value("e_ttop_catapult_buy", PF_SignalPriority::e_ttop_catapult_buy)
        .value("e_tbottom_catapult_sell", PF_SignalPriority::e_tbottom_catapult_sell);

    py::class_<PF_Signal>(m, "PF_Signal")
        .def(py::init())
        .def_readonly("signal_category_", &PF_Signal::signal_category_)
        .def_readonly("signal_type_", &PF_Signal::signal_type_)
        .def_readonly("signal_priority_", &PF_Signal::priority_)
        .def("GetSignalPrice", [](const PF_Signal& sig) { return sig.signal_price_.format("f"); });

    m.def("CmpSigPriority", &CmpSigPriority, "Compare 2 signal priorities");

    py::class_<PF_Column> PF_Col(m, "PY_PF_Column");

    PF_Col.def(py::init());

    py::enum_<PF_Column::Direction>(PF_Col, "Direction")
        .value("e_Unknown", PF_Column::Direction::e_Unknown)
        .value("e_Up", PF_Column::Direction::e_Up)
        .value("e_Down", PF_Column::Direction::e_Down);

    py::enum_<PF_Column::Status>(PF_Col, "Status")
        .value("e_Accepted", PF_Column::Status::e_Accepted)
        .value("e_Ignored", PF_Column::Status::e_Ignored)
        .value("e_Reversal", PF_Column::Status::e_Reversal)
        .value("e_AcceptedWithSignal", PF_Column::Status::e_AcceptedWithSignal);

    py::class_<PF_Chart>(m, "PY_PF_Chart")
        .def(py::init())
        .def(py::init(
            [](const std::string& symbol, std::string_view range_or_ATR, int32_t reversals) {
                return PF_Chart{symbol, sv2dec(range_or_ATR), reversals};
            }))
        .def(py::init(
            [](const std::string& symbol, std::string_view range_or_ATR, int32_t reversals, std::string_view modifier) {
                return PF_Chart{symbol, sv2dec(range_or_ATR), reversals, sv2dec(modifier)};
            }))
        .def("__str__", [](const PF_Chart& c) { return std::format("{}\n", c); })
        .def("empty", &PF_Chart::empty, "Is chart empty")
        .def(
            "GetBoxSize", [](const PF_Chart& a) { return a.GetChartBoxSize().format("f"); }, "Chart box size")
        .def("GetReversals", &PF_Chart::GetReversalboxes, "How many revesal boxes")
        .def("GetSymbol", &PF_Chart::GetSymbol)
        .def("GetCurrentDirection", &PF_Chart::GetCurrentDirection, "Chart is moving up/down/unknown")
        .def("AddValue", py::overload_cast<std::string_view, std::string_view, std::string_view>(&PF_Chart::AddValue),
             "Add new value: new value, time, time format")
        .def("AddValue", py::overload_cast<double, int64_t>(&PF_Chart::AddValue),
             "Add new value: new value, time, time format")
        .def("GetMostRecentSignal", &PF_Chart::GetMostRecentSignal)
        .def("LoadDataFromFile", &PF_Chart::LoadDataFromFile);
}

// PF_Chart(std::string symbol, decimal::Decimal base_box_size, int32_t reversal_boxes,
//         Boxes::BoxScale box_scale=Boxes::BoxScale::e_Linear,
//         decimal::Decimal box_size_modifier=0, int64_t max_columns_for_graph=0);
