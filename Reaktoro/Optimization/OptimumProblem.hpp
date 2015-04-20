// Reaktoro is a C++ library for computational reaction modelling.
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

#pragma once

// C++ includes
#include <functional>

// Reaktoro includes
#include <Reaktoro/Common/Matrix.hpp>
#include <Reaktoro/Optimization/Hessian.hpp>

namespace Reaktoro {

/// A type that describes the result of the evaluation of an objective function
struct ObjectiveResult
{
    /// The value of the objective function evaluated at `x`.
    double val;

    /// The gradient of the objective function evaluated at `x`.
    Vector grad;

    /// The Hessian of the objective function evaluated at `x`.
    Hessian hessian;
};

/// A type that describes the functional signature of an objective function.
/// @param x The vector of primal variables
/// @return The objective function evaluated at `x`
using ObjectiveFunction = std::function<ObjectiveResult(const Vector&)>;

/// A type that describes the non-linear constrained optimisation problem
struct OptimumProblem
{
    /// The objective function.
    ObjectiveFunction objective;

    /// The coefficient matrix of the linear equality constraint `Ax = b`
    Matrix A;

    /// The right-hand side vector of the linear equality constraint `Ax = b`
    Vector b;

    /// The lower bound of the primal variables `x`.
    Vector l;

    /// The upper bound of the primal variables `x`.
    Vector u;
};

} // namespace Reaktoro