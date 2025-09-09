#include "MainWindow.h"
#include "ImageProcessor.h"

#include <QAction>
#include <QFileDialog>
#include <QImageReader>
#include <QLabel>
#include <QMenuBar>
#include <QMessageBox>
#include <QPixmap>
#include <QSlider>
#include <QStatusBar>
#include <QToolBar>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QScrollArea>
#include <QResizeEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    buildUi();
    connectSignals();

    // Generate and load a default image so the app is ready out-of-the-box
    originalImage = generateDefaultImage();
    adjustedImage = originalImage;
    updatePreview();
}

void MainWindow::buildUi() {
    // Menus & toolbar
    auto *fileMenu = menuBar()->addMenu("&File");
    auto *openAct = new QAction("Open Image...", this);
    openAct->setShortcut(QKeySequence::Open);
    fileMenu->addAction(openAct);
    connect(openAct, &QAction::triggered, this, &MainWindow::openImage);

    auto *exitAct = new QAction("E&xit", this);
    exitAct->setShortcut(QKeySequence::Quit);
    fileMenu->addAction(exitAct);
    connect(exitAct, &QAction::triggered, this, &QWidget::close);

    auto *toolbar = addToolBar("Main");
    toolbar->addAction(openAct);

    // Central widget layout
    auto *central = new QWidget(this);
    auto *vbox = new QVBoxLayout(central);
    vbox->setContentsMargins(8,8,8,8);
    vbox->setSpacing(8);

    imageLabel = new QLabel(central);
    imageLabel->setAlignment(Qt::AlignCenter);
    imageLabel->setMinimumSize(320, 180);
    imageLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    imageLabel->setBackgroundRole(QPalette::Base);
    imageLabel->setAutoFillBackground(true);

    // Controls: slider + value + reset
    auto *controls = new QWidget(central);
    auto *hbox = new QHBoxLayout(controls);
    hbox->setContentsMargins(0,0,0,0);
    hbox->setSpacing(8);

    auto *label = new QLabel("Brightness:", controls);

    slider = new QSlider(Qt::Horizontal, controls);
    slider->setRange(-100, 100);
    slider->setSingleStep(1);
    slider->setPageStep(10);
    slider->setValue(0);
    slider->setTracking(true); // real-time updates

    valueLabel = new QLabel("0", controls);
    valueLabel->setMinimumWidth(30);
    valueLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    resetButton = new QPushButton("Reset", controls);
    resetButton->setToolTip("Reset brightness to 0");

    hbox->addWidget(label);
    hbox->addWidget(slider, 1);
    hbox->addWidget(valueLabel);
    hbox->addWidget(resetButton);

    vbox->addWidget(imageLabel, 1);
    vbox->addWidget(controls, 0);

    setCentralWidget(central);
    setWindowTitle("Image Viewer — Brightness Control (Qt)");
    resize(900, 600);

    statusBar()->showMessage("Ready");
}

void MainWindow::connectSignals() {
    connect(slider, &QSlider::valueChanged, this, &MainWindow::sliderChanged);
    connect(resetButton, &QPushButton::clicked, this, &MainWindow::resetBrightness);
}

void MainWindow::openImage() {
    QStringList mimeFilters;
    const QList<QByteArray> supported = QImageReader::supportedImageFormats();
    for (const auto& f : supported) mimeFilters << "*." + QString::fromLatin1(f);

    const QString path = QFileDialog::getOpenFileName(
        this,
        "Open Image",
        QString(),
        QString("Images (%1)").arg(mimeFilters.join(' '))
    );
    if (path.isEmpty()) return;

    loadImage(path);
}

void MainWindow::loadImage(const QString& path) {
    QImageReader reader(path);
    reader.setAutoTransform(true);
    QImage img = reader.read();
    if (img.isNull()) {
        QMessageBox::warning(this, "Open Image", "Failed to load image.");
        return;
    }

    // Normalize to a known fast format
    if (img.format() != QImage::Format_ARGB32 && img.format() != QImage::Format_RGB32) {
        img = img.convertToFormat(QImage::Format_ARGB32);
    }

    originalImage = img;
    currentDelta = 0;
    slider->setValue(0);
    adjustedImage = originalImage;
    updatePreview();
    statusBar()->showMessage(QString("Loaded: %1 (%2x%3)")
                             .arg(QFileInfo(path).fileName())
                             .arg(originalImage.width())
                             .arg(originalImage.height()), 3000);
}

void MainWindow::resetBrightness() {
    currentDelta = 0;
    slider->setValue(0);
    adjustedImage = originalImage;
    updatePreview();
}

void MainWindow::sliderChanged(int value) {
    valueLabel->setText(QString::number(value));
    if (value == currentDelta) return;
    currentDelta = value;
    applyBrightness(currentDelta);
}

void MainWindow::applyBrightness(int delta) {
    if (originalImage.isNull()) return;

    // Adjust brightness into adjustedImage using fast scanline access.
    adjustedImage = ImageProcessor::adjustBrightness(originalImage, delta);
    updatePreview();
}

void MainWindow::updatePreview() {
    if (adjustedImage.isNull()) return;

    // Scale to fit while keeping aspect ratio
    const QSize labelSize = imageLabel->size();
    if (labelSize.width() < 2 || labelSize.height() < 2) {
        imageLabel->setPixmap(QPixmap::fromImage(adjustedImage));
        return;
    }
    QPixmap pm = QPixmap::fromImage(adjustedImage);
    imageLabel->setPixmap(pm.scaled(labelSize, Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

void MainWindow::resizeEvent(QResizeEvent* event) {
    QMainWindow::resizeEvent(event);
    updatePreview();
}

QImage MainWindow::generateDefaultImage(int w, int h) {
    QImage img(w, h, QImage::Format_ARGB32);
    img.fill(Qt::white);

    // Create a simple colorful gradient + text
    for (int y = 0; y < h; ++y) {
        uchar* line = img.scanLine(y);
        for (int x = 0; x < w; ++x) {
            int r = (x * 255) / (w - 1);
            int g = (y * 255) / (h - 1);
            int b = ((x + y) * 255) / (w + h - 2);
            QRgb* px = reinterpret_cast<QRgb*>(line) + x;
            *px = qRgba(r, g, b, 255);
        }
    }

    QPainter p(&img);
    p.setRenderHint(QPainter::Antialiasing, true);
    p.setPen(Qt::black);
    QFont f = p.font();
    f.setPointSizeF(qMax(12.0, h / 18.0));
    f.setBold(true);
    p.setFont(f);
    p.drawText(img.rect().adjusted(0,0,0,-h/3), Qt::AlignCenter, "Qt Image Viewer");
    p.setPen(Qt::white);
    p.drawText(img.rect().adjusted(0,0,0,-h/3-2), Qt::AlignCenter, "Qt Image Viewer");
    p.end();

    return img;
}