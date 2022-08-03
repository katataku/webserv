```mermaid
classDiagram
    ConfigProcesser --> ConfigLexer : call
    ConfigProcesser --> ConfigParser : call
    ConfigProcesser --> ConfigGenerator : call
    ConfigLexer --> Token : use
    ConfigParser --> Token : use
    ConfigParser --> Node : use
    ConfigGenerator --> Node : use

    %% configファイルを解釈してWebservConfigを作成するところまでを担当 %%
    class ConfigProcesser {
      +ConfigProcesser(string path)
      +Exec()* WebservConfig
      -string path
    }

    %% configファイルの内容を字句解析するところまでを担当 %%
    class ConfigLexer {
      +ConfigLexer(string content)
      +Tokenize()* Token
      -content string
    }

    %% 軸解析後のトークンを構文解析するところまでを担当 %%
    class ConfigParser {
      +ConfigParser(Token token)
      +Parse()* Node
      -config()* Node
      -block_directive()* Node
      -single_directive()* Node
      -location_directive()* Node
      -value(Node *)* void
      -Token* token
    }

    %% 構文解析後のノードを元にWebservConfigを作成するところまでを担当 %%
    class ConfigGenerator {
      +ConfigGenerator(Node node)
      +Generate()* WebservConfig
      -GenerateWebservConfig()* WebservConfig
      -GenerateServerContext()* ServerContext
      -GenerateLocationContext()* LocationContext
      -Node* node
    }

    %% 字句解析後のトークンを表すクラス %%
    class Token {
        +NewToken(Token*, TokenKind, string)$ Token
        +Expect(Token**, string)$ Token
        +Expect(Token**, TokenKind)$ Token
        +PeekKind(Token**, TokenKind)$ bool
        +SameTokenKind(Token**, TokenKind)$ bool
        +SameToken(Token**, string)$ bool
        +Consume(Token**, string)$ bool
        +Consume(Token**, TokenKind)$ bool
        -TokenKind kind
        -string    val
        -Token     next
    }

    %% 構文解析後のノードを表すクラス %%
    class Node {
      -NodeKind     kind
      -list<string> directive_val
      -list<Node>   child_contexts
      -list<Node>   directives
    }
```

## TokenKindについて

| kind名                | 説明                   |
| -------------------- | -------------------- |
| BlockDirectiveToken  | "server"と"location"  |
| SingleDirectiveToken | "listen"とか           |
| OpenBraceToken       | "{"                  |
| CloseBraceToken      | "}"                  |
| SemicolonToken       | ";"                  |
| ValueToken           | "80"とか"www.hoge.com" |

## NodeKindの種類

| kind名                  | 説明                     |
| ---------------------- | ---------------------- |
| HttpContextNode        | "http"コンテキスト           |
| ServerContextNode      | "server"コンテキスト         |
| LocationContextNode    | "location"コンテキスト       |
| ListenDirectiveNode    | "listen"ディレクティブ        |
| AliasDirectiveNode     | "alias"ディレクティブ         |
| AutoindexDirectiveNode | "autoindex"ディレクティブ     |
| ReturnDirectiveNode    | "return"ディレクティブ        |
| CgiExtDirectiveNode    | "cgi_extension"ディレクティブ |
| ErrorPageDirectiveNode | "error_page"ディレクティブ    |

## Configファイルの文法

- パーサーはこの文法に従い、構文解析していく

```bnf
config             ::= ( block_directive | single_directive )*
block_directive    ::= ("server" | "location" value ) "{" ( single_directive | location_directive )* "}"
location_directive ::= "location" value "{" ( single_directive )* "}"
single_directive   ::= ( "listen" | "alias" | "autoindex" | "return" | "cgi_extension" | "error_page" ) value* ";"
value              ::= (英数字 | path_char | URIChar)+
path_char          ::= ( '/' | '_' | '.')
URIChar            ::= ( ':' )
```

- メタ構文の意味
  - "\[hoge\]" : hogeは0か1個
  - "hoge+" : hogeは1個以上
  - "hoge\*" : hogeは0個以上
