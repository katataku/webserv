```mermaid
classDiagram
    WebservConfig <|-- IContext
    ServerContext <|-- IContext
    LocationContext <|-- IContext

    WebservConfig "1" --> "1..*" ServerContext
    ServerContext "1" --> "1..*" LocationContext
```
