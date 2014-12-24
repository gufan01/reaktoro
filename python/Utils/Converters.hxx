// Reaktor is a C++ library for computational reaction modelling.
//
// Copyright (C) 2014 Allan Leal
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.

#include "Converters.hpp"

// Boost includes
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
using namespace boost::python;

// C++ includes
#include <iostream>
#include <vector>

namespace std {

template<typename T>
std::ostream& operator<<(std::ostream& out, const std::vector<T>& vec)
{
	out << "[";

	for(size_t i = 0; i < vec.size(); ++i)
		out << vec[i] << ((i+1 != vec.size()) ? ", " : "");

	out << "]";

	return out;
}

} // namespace std

namespace Reaktor {

template<typename T>
struct std_vector_to_python_list
{
	static PyObject* convert(std::vector<T> const& v)
	{
		list l;
		for(const T& val : v)
			l.append(val);
		return incref(l.ptr());
	}
};

template<typename T>
struct std_vector_from_python_list
{
	std_vector_from_python_list()
	{
		converter::registry::push_back(
			&convertible, &construct, type_id<std::vector<T>>());
	}

	static void* convertible(PyObject* obj_ptr)
	{
		if(!PyList_Check(obj_ptr))
			return 0;
		return obj_ptr;
	}

	static void construct(PyObject* obj_ptr,
		converter::rvalue_from_python_stage1_data* data)
	{
		extract<list> x(obj_ptr);
		if(!x.check())
			throw_error_already_set();

		list l = x();

		void *storage =
			((converter::rvalue_from_python_storage<std::vector<T>>*) data)->
				storage.bytes;

		new (storage) std::vector<T>();

		std::vector<T>& v = *reinterpret_cast<std::vector<T>*>(storage);

		for(int idx = 0; idx < len(l); ++idx)
		{
			extract<T> ext(l[idx]);
			if(!ext.check())
			{
				v.~vector<T>();
				throw_error_already_set();
			}

			v.push_back(ext());
		}

		data->convertible = storage;
	}
};

template<typename T>
void init_converter_std_vector_from_python_list()
{
	std_vector_from_python_list<T>();
}

template<typename T>
void export_std_vector(const char* type)
{
	init_converter_std_vector_from_python_list<T>();

	class_<std::vector<T>>(type)
		.def(vector_indexing_suite<std::vector<T>>());
}

template<typename T>
void export_std_vector_with_str(const char* type)
{
	init_converter_std_vector_from_python_list<T>();

	class_<std::vector<T>>(type)
		.def(vector_indexing_suite<std::vector<T>>())
		.def(self_ns::str(self_ns::self));
}

} // namespace Reaktor
