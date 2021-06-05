# Abstract
* the wrapper of gridder0

# API Pipe
* **name + _updateViewCount**
    - renew the grid count and layout
    - type: pipe
    - pipeline: qml; trigger
    - input: QJsonObject: gridder0 `scale`
    - output: input
_sample_:
```
Pipeline.run(gridder_cld.name + "_updateViewCount", {size: [5, 5]})
```
</br>
