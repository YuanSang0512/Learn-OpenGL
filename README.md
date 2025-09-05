# 更新日志

## LightTest Environment Lighting Demo

## 概述
基于 OpenGL 实现了一个立方体光照演示，重点展示 **环境光（Ambient Light）** 的效果。项目包含两个立方体：

- **Light Cube**：模拟光源，发光，不受光照影响。
- **Object Cube**：受环境光照影响的物体，颜色会随光源变化。

环境光模拟的目标是让物体在没有直接光源照射的情况下也能显示一定亮度。

---

## 功能实现

### 1. 立方体创建
- 使用统一顶点数据创建立方体，支持多立方体共享同一组顶点数据。  
- 通过不同的 **Model Matrix** 控制位置，实现多立方体渲染。

### 2. 环境光计算
物体颜色由以下公式计算：

Ambient = AmbientStrength * LightColor * ObjectColor


- `AmbientStrength`：环境光强度（0-1）  
- `LightColor`：光源颜色  
- `ObjectColor`：物体自身颜色  

C++ 示例实现：

```cpp
float ambientStrength = 0.2f;             // 环境光强度
glm::vec3 ambientColor = ambientStrength * m_lightColor;
f_toyColor = ambientColor * m_toyColor;   // 最终物体颜色
```
3. Shader 实现

Light Shader：输出光源颜色，不受光照影响。

Object Shader：根据环境光计算物体颜色输出。

4. 投影与相机

使用 透视投影，通过 glm::perspective 设置：

```cpp
m_proj = glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
```
相机位置通过 glm::lookAt 控制：
```
m_view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
```
通过旋转矩阵和位移矩阵调整物体和光源位置。

5. 渲染流程

- 绑定统一 VBO/IBO

- 绑定不同 VAO 对象绘制光源和物体

- 设置不同 Model 矩阵，实现多立方体位置控制

- 使用环境光计算物体颜色
