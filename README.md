# 📄 README / 更新日志

---

## 🔮 后续更新规划
接下来将围绕 **渲染质量提升、可扩展性与工程化** 三个方向继续推进。

### 📌 计划支持内容

#### 1. 高级 GLSL 特性
- 使用更复杂的 GLSL 结构与函数封装：
  - 自定义函数库  
  - 结构体与多光源数组  
- 为后续 PBR 与后处理打下 Shader 组织基础  

#### 2. 法线贴图（Normal Mapping）测试
- 引入切线空间（TBN 矩阵）  
- 验证法线贴图对光照细节的提升效果  
- 完成基础模型与导入模型的法线贴图测试场景  

#### 3. 伽玛矫正（Gamma Correction）
- 区分：
  - 线性空间  
  - sRGB 空间  
- 正确处理纹理采样与最终颜色输出  
- 解决亮度偏暗或过曝问题  

#### 4. HDR 渲染
- 使用浮点帧缓冲（HDR FBO）  
- 支持高动态范围颜色存储  
- 实现基础 Tone Mapping 算法用于显示  

#### 5. PBR 光照模型测试
- 引入基于物理的渲染流程：
  - Albedo  
  - Normal  
  - Metallic  
  - Roughness  
  - AO  
- 对比传统 Blinn-Phong 与 PBR 在视觉与参数调节上的差异  

#### 6. Lua 控制渲染参数
- 使用 **Lua** 脚本动态控制：
  - 光源参数  
  - 材质参数  
  - 后处理开关  
- 提升渲染器的交互性与可调试性  

#### 7. 工程结构优化
- 分离：
  - 核心渲染逻辑  
  - 功能性测试代码  
- 清理冗余与临时代码  
- 为后续功能扩展与维护提供更清晰的模块边界

---

## 2026/01/10 更新说明
本次更新围绕 **模型读取模块重构与实例化渲染支持** 展开，重点完善了 Mesh / Model 在多实例绘制场景下的数据组织与渲染流程，并新增对应测试样例用于验证正确性。

### ✨ 更新内容

#### 1. 模型读取模块重构
- 重构 Model / Mesh 的数据组织结构  
- 明确 **单 Mesh 多实例** 的使用边界，解耦模型静态数据与实例变换数据  
- 模型加载阶段仅负责：
  - 顶点数据（VBO / EBO）
  - 纹理资源绑定信息  
- 实例化相关数据延后至渲染阶段配置，提高模块复用性  

#### 2. 实例化绘制功能支持
- 新增实例化渲染路径（Instanced Rendering）：
  - 支持 `glm::mat4` 作为实例模型矩阵输入  
  - 通过额外 VBO 绑定实例矩阵数据  
  - 使用 `glVertexAttribDivisor` 实现按实例步进  
- 统一实例矩阵在 VAO 中的绑定格式，避免与普通顶点属性冲突  
- 保持普通绘制与实例化绘制接口兼容  

#### 3. InstanceTest 测试场景
- 新增 **InstanceTest** 测试样例  
- 构建批量实例模型矩阵数据，用于验证：
  - 实例矩阵正确上传至 GPU
  - 顶点属性分离与步进规则生效  
- 对比单次 DrawCall 与实例化 DrawCall 的渲染结果，确认渲染一致性  

---

## 2025/12/31 更新说明
本次更新对渲染器的 **基础设施与测试场景** 进行了系统性重构，完善了离屏渲染与天空盒相关流程。

### ✨ 更新内容

#### 1. `BasicModels` 重构
- 重构 `BasicModels` 类，**简化顶点数据获取逻辑**  
- 减少重复代码，统一基础几何体与天空盒顶点生成接口  
- 提高后续测试场景与渲染流程的复用性  

#### 2. SkyBoxTest 测试场景
- 完成 **SkyBoxTest** 测试场景  
- 验证天空盒顶点数据、Cubemap 纹理加载与渲染流程正确性  
- 确认天空盒在相机移动时保持“无限远”视觉效果  

#### 3. FrameBufferTest 测试场景
- 完成 **FrameBufferTest** 测试场景  
- 验证离屏渲染（FBO）到屏幕四边形的完整流程  
- 支持后处理 shader 扩展测试  

#### 4. 帧缓冲相关封装
- 封装 `RenderBuffer` 接口：
  - 专用于深度 / 模板附件管理  
- 封装 `FrameBuffer` 接口：
  - 统一管理颜色附件、深度附件  
  - 提供 Bind / Unbind / Check 接口，简化使用流程  

---

## 2025/12/25 更新说明
本次更新完成了 **天空盒纹理加载流程**，并扩展了基础模型生成能力。

### ✨ 更新内容

#### 1. 天空盒纹理读取
- 完成 **Cubemap 天空盒纹理读取接口**  
- 支持 6 张纹理自动加载并绑定至 `GL_TEXTURE_CUBE_MAP`  
- 正确设置纹理环绕与采样参数  

#### 2. `BasicModels` 扩展
- 在 `BasicModels` 中新增：
  - 天空盒顶点数据生成接口  
- 将天空盒几何数据统一纳入基础模型管理体系  

---

## 2025/12/22 更新说明
本次更新对 **Texture 系统** 进行了重构，以适配天空盒纹理加载需求。

### ✨ 更新内容

#### 1. `Texture` 类重构
- 重构 `Texture` 类接口  
- 添加枚举类：
  - 普通 2D 纹理
  - 后处理FBO纹理 
  - Cubemap 天空盒纹理  
- 使纹理系统能够同时支持：
  - 模型贴图
  - FBO纹理附件
  - 天空盒 Cubemap  

---

## 2025/9/17 更新说明
本次更新完成了 **BlendingTest 测试场景**，验证透明物体的正确渲染顺序。

### ✨ 更新内容

#### 1. BlendingTest 场景
- 实现 **透明物体混合渲染测试**  
- 使用标准混合函数：
  ```cpp
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

---

## 2025/9/13 更新说明
本次更新完成了 **ModelTest 场景光照**，并抽象出了 **BasicModels 类**，用于快速生成常用基础几何体（立方体等）。

### ✨ 更新内容

#### 1. `ModelTest` 场景
- 支持动态光源。
- 使用 ImGui 调整光照与材质参数：
  - 光源位置与半径  
  - 物体位置  
  - 环境光、漫反射、镜面反射强度  
  - 材质高光系数 (shininess)

#### 2. `BasicModels` 抽象
- 新增 `BasicModels` 类，封装常用几何体（Cube）：
  - `CreateCubeVertexs()`：生成带位置 / 法线 / 纹理坐标的立方体顶点  
  - `CreateCubeIndices()`：生成立方体索引缓冲
- 统一顶点结构 `Vertex`，包含：
  - `position (vec3)`
  - `normal (vec3)`
  - `texCoord (vec2)`

#### 3. 光照效果
- 实现光源立方体（`LightCube.shader`）渲染  
- 实现物体模型（`ModelTest.shader`）的 **Blinn-Phong 光照计算**  
- 通过相机控制与光源旋转，验证高光与光照变化效果  

---

## 2025/9/9 更新说明
本次更新完成了 **模型导入功能测试**，目前仅支持 **OBJ + MTL 模型格式**。

### ✨ 更新内容

#### 1. 模型导入测试
- 使用 **Assimp** 成功加载 `.obj` 模型及其 `.mtl` 材质文件  
- 验证顶点数据正确性：
  - Position  
  - Normal  
  - TexCoord  
- 支持漫反射（Diffuse）与镜面反射（Specular）纹理绑定  

#### 2. 当前限制
- 仅支持 **OBJ + MTL** 模型格式  

---

## 2025/9/7 更新说明
本次更新完成了 **Mesh / Model 类的实现**，并成功接入 **Assimp** 实现模型导入与渲染。

### ✨ 更新内容

#### 1. `Mesh` 类
- 构造函数接收：
  - 顶点数据 (Vertex)  
  - 索引数据 (Index)  
  - 纹理信息 (TextureInfo)  
- `Draw()`：
  - 支持多纹理绑定（`texture_diffuseN`、`texture_specularN`）  
  - 自动绑定纹理并传递 Shader uniform  
- `setupMesh()`：
  - 使用 `VAO / VBO / EBO` 封装网格  
  - 顶点属性布局：
    - Position (vec3)  
    - Normal (vec3)  
    - TexCoord (vec2)  

#### 2. `Model` 类
- `loadModel(path)`：使用 Assimp 加载模型  
- `processNode()`：递归解析节点  
- `processMesh()`：
  - 提取 Position / Normal / TexCoord  
  - 生成索引缓冲  
  - 解析并加载材质贴图  
- 贴图缓存机制，避免重复加载  

#### 3. `Texture` 类状态
- 原 `Texture` 类 **基本弃用**  
- 纹理加载逻辑由 `Model` 内部统一管理  

---

## 2025/9/6 更新说明
本次更新主要完成了 **相机抽象与材质系统**，并修复了镜面反射异常问题。

### ✨ 更新内容

#### 1. 相机系统（`Camera`）
- 封装：
  - 位置  
  - yaw / pitch  
  - front / up / right  
- 提供：
  - `GetViewMatrix()`  
  - `GetProjectionMatrix()`  
  - `CameraUpdate()`（鼠标控制）  
- 修复 front 向量未正确更新导致的高光异常  

#### 2. Shader 修改
- 新增 `Material` 结构体：
  ```glsl
  struct Material {
      vec3 ambient;
      vec3 diffuse;
      vec3 specular;
      float shininess;
  };
