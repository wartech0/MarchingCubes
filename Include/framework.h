#ifndef FRAMEWORK_H_
#define FRAMEWORK_H_

class Framework
{
public:
    Framework();
    ~Framework();

    void Initialize();
    void Update(double timeInSeconds);
    void Draw();
    void Destroy();
};

#endif