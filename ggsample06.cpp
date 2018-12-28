// ウィンドウ関連の処理
#include "GgApplication.h"

// シェーダー関連の処理
#include "shader.h"

// 標準ライブラリ
#include <cmath>

// アニメーションの周期（秒）
const double cycle(5.0);

//
// アプリケーションの実行
//
void GgApplication::run()
{
  // ウィンドウを作成する
  Window window("ggsample06");

  // 背景色を指定する
  glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

  // プログラムオブジェクトの作成
  const GLuint program(loadProgram("ggsample06.vert", "pv", "ggsample06.frag", "fc"));

  // in (attribute) 変数 cv のインデックスの検索（見つからなければ -1）
  const GLint cvLoc(glGetAttribLocation(program, "cv"));

  // uniform 変数のインデックスの検索（見つからなければ -1）
  const GLint mwLoc(glGetUniformLocation(program, "mw"));
  const GLint mcLoc(glGetUniformLocation(program, "mc"));
  const GLint mgLoc(glGetUniformLocation(program, "mg"));

  // ビュー変換行列を mv に求める
  const GgMatrix mv(ggLookat(0.0f, 2.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f));

  // 頂点配列オブジェクトの作成
  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  // 頂点バッファオブジェクトの作成
  GLuint vbo[2];
  glGenBuffers(2, vbo);

  // 頂点の座標値
  static const GLfloat pv[][3] =
  {
    { -1.0f, -0.8660254f, 0.0f },
    {  1.0f, -0.8660254f, 0.0f },
    {  0.0f,  0.8660254f, 0.0f }
  };

  // 頂点の数
  static const int points(sizeof pv / sizeof pv[0]);

  // 頂点の座標値 pv 用のバッファオブジェクト
  glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof pv, pv, GL_STATIC_DRAW);

  // 結合されている頂点バッファオブジェクトを in 変数 pv (index == 0) から参照できるようにする
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(0);

  // 頂点の色
  static const GLfloat cv[][3] =
  {
    { 1.0f, 0.0f, 0.0f },  // 赤
    { 0.0f, 1.0f, 0.0f },  // 緑
    { 0.0f, 0.0f, 1.0f }   // 青
  };

  // 頂点の法線ベクトル（宿題用）
  static const GLfloat nv[][3] =
  {
    { -0.086172748f, -0.049751860f, 0.99503719f },
    {  0.086172748f, -0.049751860f, 0.99503719f },
    {  0.0f, 0.099503719f, 0.99503719f }
  };

  // 頂点の色 cv 用のバッファオブジェクト
  glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
  // 【宿題】cv の代わりに nv を使うように変更する
  glBufferData(GL_ARRAY_BUFFER, sizeof cv, cv, GL_STATIC_DRAW);

  // 結合されている頂点バッファオブジェクトを in 変数 cv (index == cvLoc) から参照できるようにする
  glVertexAttribPointer(cvLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(cvLoc);

  // 経過時間のリセット
  glfwSetTime(0.0);

  // ウィンドウが開いている間繰り返す
  while (window)
  {
    // ウィンドウを消去する
    glClear(GL_COLOR_BUFFER_BIT);

    // シェーダプログラムの使用開始
    glUseProgram(program);

    // 時刻の計測
    const float t(static_cast<float>(fmod(glfwGetTime(), cycle) / cycle));

    // モデルビュー変換行列 (時刻 t にもとづく回転アニメーション)
    const GgMatrix mw(mv.rotateY(12.56637f * t));

    // 法線変換行列
    const GgMatrix mg(mw.normal());

    // 投影変換行列
    const GgMatrix mp(ggPerspective(0.5f, window.getAspect(), 1.0f, 15.0f));

    // モデルビュー・投影変換
    const GgMatrix mc(mp * mw);

    // uniform 変数を設定する
    glUniformMatrix4fv(mwLoc, 1, GL_FALSE, mw.get());
    glUniformMatrix4fv(mcLoc, 1, GL_FALSE, mc.get());
    glUniformMatrix4fv(mgLoc, 1, GL_FALSE, mg.get());

    // 描画に使う頂点配列オブジェクトの指定
    glBindVertexArray(vao);

    // 図形の描画
    glDrawArrays(GL_TRIANGLES, 0, points);

    // 頂点配列オブジェクトの指定解除
    glBindVertexArray(0);

    // シェーダプログラムの使用終了
    glUseProgram(0);

    // カラーバッファを入れ替えてイベントを取り出す
    window.swapBuffers();
  }
}

