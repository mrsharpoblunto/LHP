LHP
===

LHP is a Framework for writing MVC web applications in c++ using Lua for page templating. The design of the framework is heavily inspired by the ASP.NET MVC framework, and considering this was heavily derived from Rails and other MVC frameworks, the overall idea should be relatively familar to most people who have used an MVC framework before.

Controllers
-----------

Just as in ASP.NET MVC, LHP has the concept of Controllers which have a number of action methods. These controller actions are then registered so that when a URL is to be processed on the server, the correct action is invoked. The only required method on a controller class is the Register method which as shown below allows you to register callback functions for controller actions, these can take the form of free functions, static member functions or c++ lambdas. An example controller is shown below

```c++
void TestController::Register(LHP::ApplicationController &controller)
{
  controller.RegisterControllerGetAction("Test","Index",&TestController::Index);
  controller.RegisterControllerFormPostAction("Test","Submit",&TestController::Submit);
}

void TestController::Index(const LHP::ControllerContext &context,const std::string id,
  const LHP::Server::Request &request,LHP::Server::Response &response)
{
  //controller action code goes here
}

void TestController::Submit(const LHP::ControllerContext &context,const LHP::HtmlForm &form,
  const LHP::Server::Request &request,LHP::Server::Response &response)
{
  //controller action code goes here
}
```

Models
------

Now also just like ASP.NET MVC LHP has the concept of models and views. The models are created by controllers and are passed though to the view engine which is uses Lua for HTML preprocessing.

Models are plain c++ objects with the addition of a bind method which is used to expose the objects properties to Lua - this is done using the luabind library. An example Model is shown below

```c++

//TestModel.hpp
class TestModel: public LHP::IModel
{
public:
  virtual ~TestModel(){}
  std::string GetValue() const;
  void SetValue(std::string value);
  virtual void Bind(lua_State *state);
private:
  std::string _value;
};

//TestModel.cpp implementation of bind method
void TestModel::Bind(lua_State *state)
{
  luabind::module(state)
  [
    luabind::class_<TestModel>("TestModel")
    .property("Value", &TestModel::GetValue,&TestModel::SetValue)
  ];
}
```

This model class would be created and passed to the view engine for rendering in a controller action as follows

```c++  
TestModel testModel;
testModel.SetValue("world");
LHP::ViewData<TestModel> viewData(testModel);

RenderHtml<TestModel>(context,viewData,response);
```

Views
-----

Finally we have views which are stored in .lhp files. These views are used to render the final markup to the client and are able to use lua to dynamically generate this content. The view's lua code also has access to any of the models properties which were bound to lua in the previous step. Views also support the concept of 'Master pages' which define markup that is common to multiple views. An example view and master page is shown below.

Master Page
-----------

```html
<html>
<head>
<title><@placeholder=title@></title>
</head>
<body>
<@placeholder=content@>
</body>
</html>
```

LHP View Template
-----------------

```lua
<@masterpage=Shared/site.master@>

<@placeholder=title@>
<%
function Hello(name)
return "Hello " .. name .. "!\n"
end

function Title(name)
return "LHP::" .. name .. "!\n"
end
%>
<%=Title("Test Page")%>
<@endplaceholder@>

<@placeholder=content@>
<h1><%=Title("Test Page")%></h1>
<%
response:write(Hello(viewData.Value) .. "<br/>")
%>
<@endplaceholder@>
```

Compiling the source
--------------------

To build the solution you will need Visual Studio 2010. You will also need the boost libraries installed, and you will need to add the following environment variable

    BOOST_DIR
    
Which should point to the location on disk where you have the boost libraries installed.

Running the application
-----------------------

LHP comes with an example test application that runs using LHP's standalone web server. To run it you'll need to run 


    TestApplication.exe <ip> <port> <number of threads>

An example configuration would be

    TestApplication.exe 0.0.0.0 8080 1
    
Once you have the server up and running you should be able to go to a browser and enter

    http://localhost:8080/Test/
    
and see the test page! 

NOTE: LHP is very much a proof of concept, and needs much more work to be fully usable. However its MIT licensed, so free to use it/modify it as you see fit :)

  