// NetException.h
//
// Author(s):       kNet Authors <https://github.com/juj/kNet>
//                  iFarbod <>
//
// Copyright (c) 2015-2017 CtNorth Team
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

/** @file NetException.h
    @brief The class NetException. Common exception class thrown by kNet on errors. */
#include <exception>
#include <string>

namespace kNet
{

class NetException : public std::exception
{
public:
    explicit NetException(const char *reason)
    :exception(reason)
    {
    }

    ~NetException() throw()
    {
    }

    const char *what() const throw() { return exception.c_str(); }

private:
    std::string exception;
};

} // ~kNet
