#pragma once
#include <QMainWindow>
#include <QImage>
#include <QPointer>

class QLabel;
class QSlider;
class QPushButton;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

protected:
    void resizeEvent(QResizeEvent* event) override;

private slots:
    void openImage();
    void resetBrightness();
    void sliderChanged(int value);

private:
    void buildUi();
    void connectSignals();
    void loadImage(const QString& path);
    void applyBrightness(int delta);
    void updatePreview();
    QImage generateDefaultImage(int w = 640, int h = 360);

    QPointer<QLabel> imageLabel;
    QPointer<QSlider> slider;
    QPointer<QLabel> valueLabel;
    QPointer<QPushButton> resetButton;

    QImage originalImage;   // Original loaded image
    QImage adjustedImage;   // Image after applying brightness
    int currentDelta = 0;
};