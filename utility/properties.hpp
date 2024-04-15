 class WindowProperties{
private:
    float m_window_size_x;
    float m_window_size_y;
public:
    float getWindowX(){
        return m_window_size_x;
    }
    float getWindowY(){
        return m_window_size_y;
    }
    void setWindowSize(float x, float y){
        m_window_size_x = x;
        m_window_size_y = y;
    }
 };
 WindowProperties window_properties;