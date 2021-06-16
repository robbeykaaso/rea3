# Abstract
* provide some common components for user  
* one component basically includes basic prototype(a layout, some static styles and the business logic), style(a style singleton for dynmamic styles) and pipe interface(the interface based on rea)  

# Design
* we extract the pipe interface for the developer who only want to use the component but not based on rea in his project  
* we extract the style for the convenient management and customization of the component style. we use singleton rather than class-object for the real-time dynamic property mapping logic provided by qml  