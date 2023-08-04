import unittest

import os.path
import sys
sys.path.insert(0, "/home/dpriedel/projects/PF_Project/PY_PF_Chart")

import PY_PF_Chart

class TestChart_default_ctor(unittest.TestCase):

    def test_create_chart(self):
        py_chart = PY_PF_Chart.PY_PF_Chart()

        self.assertNotEqual(py_chart, None)

class TestChart_ctor_with_args(unittest.TestCase):

    def setUp(self):
        self.py_chart = PY_PF_Chart.PY_PF_Chart("AAPL", "10", 2)

    def test_create_chart_with_options(self):
        self.assertNotEqual(self.py_chart, None)

    def test_chart_isempty(self):
        self.assertTrue(self.py_chart.empty())

    def test_create_get_symbol(self):
        self.assertEqual(self.py_chart.GetSymbol(), "AAPL")

    def test_create_get_box_size(self):
        self.assertEqual(self.py_chart.GetBoxSize(), "10")

    def test_create_get_reversals(self):
        self.assertEqual(self.py_chart.GetReversals(), 2)

class Test_add_values_to_chart(unittest.TestCase):

    def setUp(self):
        self.py_chart = PY_PF_Chart.PY_PF_Chart("AAPL", "10", 2)

    def test_01_add_first_value(self):
        # print(self.py_chart)
        self.py_chart.AddValue("100", "2023-06-01", "%Y-%m-%d")
        self.assertFalse(self.py_chart.empty())
        self.assertEqual(self.py_chart.GetCurrentDirection(), PY_PF_Chart.PY_PF_Column.Direction.e_Unknown)
        # print(self.py_chart)

    def test_02_ignore_next_value(self):
        # print("\nbefore: ", self.py_chart)
        self.py_chart.AddValue("100", "2023-06-01", "%Y-%m-%d")
        # print("\nafter first: ", self.py_chart)
        result = self.py_chart.AddValue("100", "2023-06-02", "%Y-%m-%d")
        # print("\nafter second. s.b. ignored: ", self.py_chart)
        self.assertEqual(result, PY_PF_Chart.PY_PF_Column.Status.e_Ignored)
        # this value should also be ignored because it is the same date as the first entry and thus 'old' data
        result = self.py_chart.AddValue("110", "2023-06-01", "%Y-%m-%d")
        self.assertEqual(result, PY_PF_Chart.PY_PF_Column.Status.e_Ignored)
        self.assertEqual(self.py_chart.GetCurrentDirection(), PY_PF_Chart.PY_PF_Column.Direction.e_Unknown)

    def test_03_direction_is_up(self):
        self.py_chart.AddValue("100", "2023-06-01", "%Y-%m-%d")
        self.py_chart.AddValue("111", "2023-06-03", "%Y-%m-%d")
        # print(self.py_chart)
        self.assertEqual(self.py_chart.GetCurrentDirection(), PY_PF_Chart.PY_PF_Column.Direction.e_Up)

class Test_add_values_to_chart_from_file(unittest.TestCase):

    def setUp(self):
        self.py_chart = PY_PF_Chart.PY_PF_Chart("AAPL", "2", 2)

    def test_01_first_file(self):
        file_name = os.path.expanduser("~/projects/PF_Project/PF_Test/test_files/AAPL_close.dat")
        self.py_chart.LoadDataFromFile(file_name, "%Y-%m-%d", ",")
        print(self.py_chart)

if __name__ == '__main__':
    unittest.main()

