# 結合テストの説明

## 確認項目

- header lineの一致確認
- Content-Typeの一致確認
- Content-Lengthの一致確認
- Transfer-Encodingの一致確認
- `success</p>`の一致確認（ファイルアップロード成功時にCGIプログラムから生成される文言。POSTが成功したかどうかの確認に代替する。）
