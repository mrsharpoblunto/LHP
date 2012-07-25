#pragma once

namespace LHP {

/** 
strongly typed container for storing a bound model object to 
be passed to a lua template view
*/
template <typename T> class ViewData
{
public:
	virtual ~ViewData(){}
	ViewData(T &model) : _model(model) {}
	T &GetModel() const { return _model; }
private:
	T &_model;
};

}