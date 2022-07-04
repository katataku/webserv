# 結合テストの説明

## configファイル

- 0001: 通常の設定。
- 0002: 現状はダミー(0001と同じ内容。結合テスト基盤の動作確認用。)

## request

- 0001: サンプルhtmlへのGETリクエスト。
- 0002: ファイルアップロードCGIへのPOSTリクエスト。
- 0003: サンプルhtmlへのGETリクエスト。(ヘッダ`expect:  100-continue` 付き)
    <https://developer.mozilla.org/ja/docs/Web/HTTP/Headers/Expect>

## 確認項目

- header lineの一致確認
- Content-Typeの一致確認
- Content-Lengthの一致確認
- Transfer-Encodingの一致確認
- `success</p>`の一致確認（ファイルアップロード成功時にCGIプログラムから生成される文言。POSTが成功したかどうかの確認に代替する。）
