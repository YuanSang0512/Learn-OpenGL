# 📄 README / 更新日志

## 📑 目录
- [9月5日 更新说明](#202595-更新说明)
- [9月6日 更新说明](#202596-更新说明)
- [9月7日 更新说明](#202597-更新说明)
- [9月9日 更新说明](#202599-更新说明)
- [9月13日 更新说明](#202513-更新说明)

## 2025/9/5 更新说明
本次更新实现了一个 **OpenGL 光照测试程序**，主要包含：
- 一个可渲染的立方体（带漫反射、环境光、镜面反射效果）。
- 一个小立方体代表光源，沿圆周轨迹旋转。
- 基于 **Blinn-Phong 光照模型** 的片段着色器。

---

## ✨ 更新内容

### 1. Shader 修改
- **Vertex Shader**
  - 输入：`position`、`normal`。
  - 计算片段位置 `m_FragPos` 和修正法线 `m_Normal`。
  - 输出光照相关参数（光源颜色、位置，环境光强度，摄像机位置，镜面反射强度等）。
  - 确保法线在变换后仍然正确：  
    ```glsl
    m_Normal = normalize(mat3(transpose(inverse(u_Model))) * normal);
    ```

- **Fragment Shader**
  - 环境光：  
    ```glsl
    vec3 ambient = m_AmbientStrength * m_lightColor;
    ```
  - 漫反射：  
    ```glsl
    float diff = max(dot(m_Normal, lightDir), 0.0);
    vec3 diffuse = diff * m_lightColor;
    ```
  - 镜面反射：采用 **Blinn-Phong 半程向量**，并避免背面错误高光：  
    ```glsl
    if(diff > 0.0) {
        vec3 halfwayDir = normalize(lightDir + viewDir);
        float spec = pow(max(dot(m_Normal, halfwayDir), 0.0), 128);
        specular = m_specularStrength * spec * m_lightColor;
    }
    ```
  - 最终颜色：  
    ```glsl
    vec3 result = ambient * m_Color + diffuse * m_Color + specular;
    ```

---

### 2. C++ 渲染逻辑修改 (`LightTest.cpp`)
- **立方体生成**
  - `CreateCube`：生成立方体顶点数据（24 个顶点，每面独立法线 → 硬边立方体）。
  - `CreateCubeIndices`：生成索引数据（36 个索引）。
- **光源运动**
  - 光源小立方体位置基于时间沿圆周旋转：  
    ```cpp
    lightPos.x = cos(theta) * radius;
    lightPos.z = sin(theta) * radius;
    ```
- **渲染流程**
  - 清除颜色 & 深度缓冲。
  - 绘制光源立方体（缩放后的小 cube）。
  - 绘制物体立方体，并传递所有光照 uniform（颜色、位置、相机位置等）。
- **交互控制 (ImGui)**
  - 支持调节光源位置、物体位置、光源颜色、物体颜色。
  - 支持相机旋转角度 (rot_x, rot_y, rot_z)。
  - 支持环境光强度 `ambientStrength`、镜面反射强度 `specularStrength`。

---

## 2025/9/6 更新说明
本次更新主要完成了 **相机抽象与材质系统**，并修复了镜面反射异常的 bug，具体包括：
- 新增 `Camera` 类，支持鼠标控制视角。
- 在 **Phong 光照模型** 中加入 **材质 (Material)** 概念。
- 修复了因 **相机 front 向量未正确更新** 导致的高光异常。

---

## ✨ 更新内容

### 1. 相机抽象 (`Camera.h / Camera.cpp`)
- 定义 `Camera` 类，封装位置、方向（yaw、pitch）、`front`、`up`、`right` 等属性。
- 提供：
  - `GetViewMatrix()`：返回 `glm::lookAt` 计算的视图矩阵。
  - `GetProjectionMatrix()`：基于 `glm::perspective` 生成投影矩阵。
  - `CameraUpdate()`：根据鼠标输入更新 yaw/pitch，修正 `front` 向量。
- 修复 **front 向量未能正确更新** 的问题，解决镜面反射高光异常。

---

### 2. Shader 修改

- **Fragment Shader**
  - 新增 `Material` 结构体：
    ```glsl
    struct Material {
        vec3 ambient;
        vec3 diffuse;
        vec3 specular;
        float shininess;
    };
    ```
  - 光照计算公式更新：
    - 环境光：  
      ```glsl
      vec3 f_ambient = u_Light.ambient * u_material.ambient;
      ```
    - 漫反射：  
      ```glsl
      float diff = max(dot(norm, lightDir), 0.0);
      vec3 f_diffuse = u_Light.diffuse * (diff * u_material.diffuse);
      ```
    - 镜面反射（Blinn-Phong 半程向量）：  
      ```glsl
      if(diff > 0.0) {
          vec3 viewDir = normalize(u_CamPos - m_FragPos);
          vec3 halfwayDir = normalize(lightDir + viewDir);
          float spec = pow(max(dot(norm, halfwayDir), 0.0), u_material.shininess);
          f_specular = u_Light.specular * u_material.specular * spec;
      }
      ```
    - 最终颜色：  
      ```glsl
      vec3 result = f_ambient + f_diffuse + f_specular;
      ```

---

### 3. C++ 渲染逻辑修改 (`LightTest.cpp`)
- **物体材质**
  - 使用 `u_material` 传递材质参数（ambient / diffuse / specular / shininess）。
  - 可通过 ImGui 实时调节材质与光照强度。
- **相机控制**
  - 使用 `Camera` 类管理相机视图 & 投影矩阵。
  - 支持键盘、鼠标交互调整视角。

---

## 2025/9/7 更新说明
本次更新完成了 **Mesh / Model 类的实现**，并成功接入 **Assimp 库** 实现模型导入与渲染。由于 `Model` 与 `Mesh` 类已经完整接管了纹理加载流程，之前的 `Texture` 类基本弃用。

---

## ✨ 更新内容

### 1. `Mesh` 类
- 新增 `Mesh` 构造函数，接收 **顶点 (Vertex)**、**索引 (Index)** 和 **纹理 (TextureInfo)** 数据。  
- `Draw()` 方法：
  - 支持多纹理绑定（`texture_diffuseN`、`texture_specularN`）。
  - 在绘制前自动激活纹理单元并传递 `Shader` uniform。
  - 使用 `Renderer` 渲染绑定的 `VAO` / `EBO`。
- `setupMesh()` 方法：
  - 使用 `VertexArray` / `VertexBuffer` / `IndexBuffer` 封装网格数据。
  - 定义了 **顶点属性布局**：
    - 位置 (vec3)  
    - 法线 (vec3)  
    - 纹理坐标 (vec2)

---

### 2. `Model` 类
- `loadModel(path)`：使用 **Assimp** 导入模型文件（支持三角化、纹理坐标翻转）。
- `processNode(node, scene)`：递归解析 **节点及子节点**，并存储对应 `Mesh`。
- `processMesh(mesh, scene)`：
  - 提取顶点数据：`Position`、`Normal`、`TexCoord`。
  - 生成索引缓冲。
  - 解析材质，加载 `Diffuse` / `Specular` 贴图。
- `loadMaterialTextures`：材质贴图加载与缓存，避免重复读取。
- `TextureFromFile`：使用 `stb_image` 读取纹理并生成 OpenGL 纹理对象，支持 `RGB` / `RGBA`。

---

### 3. `Texture` 类状态
- `Texture` 类已 **基本弃用**，不再直接使用。
- 纹理加载逻辑由 `Model` 内的 `TextureFromFile` 与 `TextureInfo` 接管。

---

## 2025/9/9 更新说明
本次更新完成了 **模型导入功能测试**，目前仅支持 **OBJ + MTL 模型格式**。  

---

## ✨ 更新内容

### 1. 模型导入测试
- 使用 Assimp 成功加载 **.obj** 模型及其对应 **.mtl 材质文件**。  
- 验证了顶点数据（Position、Normal、TexCoord）与索引缓冲的正确性。  
- 支持漫反射 (Diffuse) 与镜面反射 (Specular) 纹理绑定。  

### 2. 限制说明
- 当前版本仅支持 **OBJ + MTL** 模型格式。  

---

## 2025/9/13 更新说明
本次更新完成了 **ModelTest 场景光照**，并抽象出了 **BasicModels 类**，用于快速生成常用基础几何体（立方体等）。  

---

## ✨ 更新内容

### 1. `ModelTest` 场景
- 支持动态光源。
- 使用 ImGui 调整光照与材质参数：  
  - 光源位置与半径  
  - 物体位置  
  - 环境光、漫反射、镜面反射强度  
  - 材质高光系数 (shininess)

### 2. `BasicModels` 抽象
- 新增 `BasicModels` 类，封装常用几何体（Cube）：  
  - `CreateCubeVertexs()`：生成带位置 / 法线 / 纹理坐标的立方体顶点。  
  - `CreateCubeIndices()`：生成立方体索引缓冲。  
- 统一了顶点结构 `vertex`，包含：  
  - `position (vec3)`  
  - `normal (vec3)`  
  - `texCoord (vec2)`  

### 3. 光照效果
- 实现光源立方体 (`LightCube.shader`) 渲染。  
- 实现物体模型 (`ModelTest.shader`) 的 Blinn-Phong 光照计算。  
- 通过相机控制与光源旋转，验证了高光与阴影的动态效果。  
