#pragma once

namespace LHP {

	class PreprocessorError: public std::exception
	{
	public:
		virtual ~PreprocessorError(){}
		PreprocessorError(const std::string &what);
	};

}