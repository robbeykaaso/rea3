# Abstract
a tool which supports undo and redo command  

# API Pipe
* **addCommand**  
    - add a command to the command manager. its type is pipePartial
    - input: ICommand  
    - output: ICommand  
    - type: pipePartial    
</br>

* **doCommand**  
    - un/re do command. if the input is greater than 0, the command will be redo, else undo. its type is pipePartial  
    - input: double
    - output: double
    - type: pipePartial
</br>  
