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
        +Consume(Token**, string)$ Token
        +Consume(Token**, TokenKind)$ Token
        +PeekKind(Token**, TokenKind)$ bool
        +SameTokenKind(Token**, TokenKind)$ bool
        +SameToken(Token**, string)$ bool
        +Expect(Token**, string)$ bool
        +Expect(Token**, TokenKind)$ bool
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
| BlockDirectiveToken  | "server"             |
| SingleDirectiveToken | "listen"とか           |
| OpenBraceToken       | "{"                  |
| CloseBraceToken      | "}"                  |
| SemicolonToken       | ";"                  |
| ValueToken           | "80"とか"www.hoge.com" |

## NodeKindの種類

| kind名               | 説明               |
| ------------------- | ---------------- |
| HttpContextNode     | "http"コンテキスト     |
| ServerContextNode   | "server"コンテキスト   |
| LocationContextNode | "location"コンテキスト |
| ListenDirectiveNode | "listen"ディレクティブ  |
| AliasDirectiveNode  | "alias"ディレクティブ   |
| AutoindexDirectiveNode  | "autoindex"ディレクティブ   |

## Configファイルの文法

- パーサーはこの文法に従い、構文解析していく

<!-- TOOD(iyamada) block_directive, single_directive, location_directiveは一つしかパースできない -->

```
config             ::= block_directive
block_directive    ::= ("server" | "location" value ) "{" ( single_directive | location_directive ) "}"
location_directive ::= "location" value "{" ( single_directive )+ "}"
single_directive   ::= ( "listen" | "alias" | "autoindex" ) value ";"
value              ::= (英数字 | ".")+
```

- メタ構文の意味
  - "\[hoge\]" : hogeは0か1個
  - "hoge+" : hogeは1個以上

## 擬似コード

```cpp

class ConfigLexer {
  public:
    // 字句解析する対象のコンテンツを受け取る
    ConfigLexer(string content)

    // 字句解析を行いトークンを得る
    Token Tokenize() {
      for(;;) {
        SkipWhiteSpaces() // SPとかNLをスキップ
        if keyword is "server" {
          MakeServerToken()
          UpdateToken()
          continue
        }
        if keyword is "{" {
          MakeOpenBraceToken()
          UpdateToken()
          continue
        }
        if keyword is "}" {
          MakeCloseBraceToken()
          UpdateToken()
          continue
        }
        // 不適切なキーワードはエラー
        throw UnknownKeyword()
      }
    }

  private:
    string content
}

class ConfigParser {
  public:
    // 構文解析する対象のトークンを受け取る
    ConfigParser(Token token)

    // 構文解析を行い、トークンを別のデータ構造に変換しつつ構文チェックを行う
    void Parse() {
      node = config()
    }

  private:
    Token token

    Node block_directive() {
      if (token.Expect("server")) {
        MakeServerNode()
        UpdateServerNode()
        if (token.Expect("{")) {
          throw NeedOpenBrace()
        }
        node = single_directive()
        if (token.Expect("}")) {
          throw NeedOpenBrace()
        }
        return node
      }
      throw hoge()
    }

    Node config() {
      return block_directive()
    }
}

class ConfigGenerator {
  public:
    // WebservConfigに詰めるノードを受け取る
    ConfigGenerator(Node node)

    // ノードを辿り、WebservConfigを作成する
    WebservConfig Generate(Node* node) {
        return GenerateWebservConfig(node)
    }
  private:
    Node node

    WebservConfig GenerateWebservConfig(Node* node) {
        WebservConfig conf

        for directive in node.directives {
            if directive is error_page {
                conf.SetErrorPage(directive.Value())
            }
            if directive is limit_except {
                conf.SetLimitExcept(directive.Value())
            }
            ...
        }
        for child_context in node.child_contexts {
            if child_context is server_context {
                conf.AddServerContext(GenerateServerContext(node))
                continue
            }
            throw UnknownContext()
        }
        return 
    }

    ServerContext GenerateServerContext(Node* node) {
        ServerContext serv

        for directive in node.directives {
            if directive is server_name {
                serv.SetServerName(directive.Value())
            }
            if directive is redirect_url {
                serv.SetRedirectURL(directive.Value())
            }
            ...
        }
        for child_context in node.child_contexts {
            if child_context is location_context {
                serv.SetLocationContext(GenerateLocationContext(node))
                continue
            }
            throw UnknownContext()
        }

        return serv
    }

    LocationContext GenerateLocationContext(Node* node) {
        LocationContext location

        for directive in node.directives {
            if directive is alias {
                location.SetAlias(directive.Value())
            }
            ...
        }
        if node.HasContext() {
            throw UnknownContext()
        }
        return location
    }
}

class ConfigProcesser {
  public:
    // configファイルのパスを受け取る
    ConfigProcesser(string path)

    // configファイルを解釈してWebservConfigを生成
    WebservConfig Exec() {
      content = ReadFile(path)
      ConfigLexer lexer(content)
      Token token = lexer.Tokenize()
      ConfigParser parser(token)
      Node node = parser.Parse()
      ConfigGenerator generator(node)
      return generator.Generate()
    }

  private:
    // configファイルのパス
    string path
}

class Token {
  public:
    // 次のトークンに進む。
    void Consume()

    // 次のトークンを返す。トークンは進めない
    Token Peek()

    // 次のトークンが期待されるトークンかを判定する
    // 期待されるトークンだと次に進む
    bool Expect(string expect_val)

  private:
    TokenKind kind
    string val
    Token next
}

class Node {
  public:

  private:
    // いま見ているコンテキストが持つコンテキスト
    Node next_context

    // いま見ているコンテキストが持つディレクティブ
    Node next_directive

    // いま見ているコンテキストの種類
    NodeKind context_kind

    // いま見ているディレクティブの種類
    NodeKind directive_kind

    // いま見ているディレクティブの値
    list<string> directive_val
}

```
