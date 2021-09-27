# Abstract
* a tool which supports undo and redo command  

# Interface  
* **ICommand(std::function<bool(void)\> aRedo, std::function<bool(void)\> aUndo)**  
    - construct a command  
    - `aRedo` a redo function  
    - `aUndo` a undo function  
</br>

# API Pipe
* **addCommand**  
    - add a command to the command manager  
    - type: pipePartial  
    - pipeline: c++; trigger  
    - input: ICommand  
    - output: input  
</br>

* **doCommand**  
    - un/re do command  
    - type: pipePartial  
    - pipeline: c++; trigger  
    - input: bool : true=redo : false=undo  
    - output: input  
</br>  
