# 📄 README / 更新日志

## 2025/9/5更新说明
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

## 📝 后续改进方向
1. **添加 shininess uniform**  
   目前写死为 `128`，建议通过 `ImGui` 加 slider 控制。
2. **平滑法线版本立方体**  
   若需要点状高光，而非条带，需要实现 8 顶点版本 + 平均法线。
3. **PBR 光照模型**  
   后续可尝试 Cook-Torrance（roughness/metallic 支持）。
4. **多光源支持**  
   扩展 Shader 输入，支持点光源、平行光、聚光灯。
