#include "StdAfx.h"

#include "PreprocessorError.hpp"

namespace LHP {

PreprocessorError::PreprocessorError(const std::string &what) : std::exception(what.c_str())
{
}

}