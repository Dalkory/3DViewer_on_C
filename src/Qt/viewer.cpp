#include "viewer.h"

#include <chrono>
#include <cstdint>
#include <thread>

#include "gif.h"
#include "ui_viewer.h"

// Конструктор класса Viewer.
Viewer::Viewer(QWidget *parent) : QMainWindow(parent), ui(new Ui::Viewer) {
  ui->setupUi(this);  // Устанавливаем пользовательский интерфейс для этого
                      // виджета с помощью Ui::Viewer
  setWindowTitle("3DViewer");  // Задаем заголовок окна
  scene = new QGraphicsScene();  // Создаем новую графическую сцену
  ui->Widget_3D->setScene(scene);  // Назначаем эту сцену на наш 3D виджет
  isGifRecording = false;  // отключаем запись GIF по умолчанию
  isModelExist = 0;
  scene->setSceneRect(-(max_W / 2), -(max_H / 2), max_W,
                      max_H);  // Задаем область отображения сцены
  ui->Widget_3D->viewport()->installEventFilter(
      this);  // Устанавливаем фильтр событий для видового экрана (viewport)
              // виджета
  ui->Widget_3D->setVerticalScrollBarPolicy(
      Qt::ScrollBarAlwaysOff);  // Выключаем вертикальную полосу прокрутки
  ui->Widget_3D->setHorizontalScrollBarPolicy(
      Qt::ScrollBarAlwaysOff);  // Выключаем горизонтальную полосу прокрутки
  ui->statusbar->showMessage(
      "Open *.obj file ");  // Отображаем начальное сообщение в строке состояния
  setlocale(LC_ALL, "en_US.UTF-8");  // Устанавливаем региональные настройки на
                                     // английский язык
  set_settings_from_data_to_window(&settings);
}

Viewer::~Viewer() {
  save_settings_from_window(&settings);
  delete ui;
}

void Viewer::get_str(char *input_str, QString src) {
  QByteArray ba = src.toLocal8Bit();
  char *str = ba.data();
  int i;
  for (i = 0; i < 255 && *str; i++) {
    *input_str = *str;
    str++;
    input_str++;
  }
  *input_str = '\0';
}

void Viewer::drawLines() {
  scene->clear();
  ui->Widget_3D->centerOn(0, 0);
  pen_line.setColor(colorLine);
  pen_line.setWidth(lineWidth);

  for (unsigned int i = 1; i <= data.facets_count; i++) {
    if (i >= 1 && i < data.facets_count + 1 &&
        data.facets_data.facets_vertexes[i][0] >= 1 &&
        data.facets_data.facets_vertexes[i][0] < data.vertexes_data.rows) {
      if (currentProjection == Parallel) {
        x0 = data.vertexes_data.A[data.facets_data.facets_vertexes[i][0]][0] *
             scale;
        y0 = -data.vertexes_data.A[data.facets_data.facets_vertexes[i][0]][1] *
             scale;
      } else {
        x0 = (d *
              data.vertexes_data.A[data.facets_data.facets_vertexes[i][0]][0]) /
             (data.vertexes_data.A[data.facets_data.facets_vertexes[i][0]][2] +
              d) *
             scale;
        y0 = (d *
              data.vertexes_data.A[data.facets_data.facets_vertexes[i][0]][1]) /
             (data.vertexes_data.A[data.facets_data.facets_vertexes[i][0]][2] +
              d) *
             scale;
      }
      x1 = x0;
      y1 = y0;
      for (int j = 1; j < data.facets_data.number_of_vertexes[i]; j++) {
        if (data.facets_data.facets_vertexes[i][j] >= 1 &&
            data.facets_data.facets_vertexes[i][j] < data.vertexes_data.rows) {
          if (currentProjection == Parallel) {
            x2 = data.vertexes_data
                     .A[data.facets_data.facets_vertexes[i][j]][0] *
                 scale;
            y2 = -data.vertexes_data
                      .A[data.facets_data.facets_vertexes[i][j]][1] *
                 scale;
          } else {
            x2 = (d * data.vertexes_data
                          .A[data.facets_data.facets_vertexes[i][j]][0]) /
                 (data.vertexes_data
                      .A[data.facets_data.facets_vertexes[i][j]][2] +
                  d) *
                 scale;
            y2 = (d * data.vertexes_data
                          .A[data.facets_data.facets_vertexes[i][j]][1]) /
                 (data.vertexes_data
                      .A[data.facets_data.facets_vertexes[i][j]][2] +
                  d) *
                 scale;
          }
          scene->addLine(x1, y1, x2, y2, pen_line);
          x1 = x2;
          y1 = y2;
        }
      }
      scene->addLine(x1, y1, x0, y0, pen_line);
    }
  }
}

void Viewer::drawVertices() {
  vertexItems.clear();
  vertexSquareItems.clear();
  vertexPen.setColor(colorVertexes);
  for (unsigned int i = 1; i <= data.vertex_count; ++i) {
    if (currentProjection == Parallel) {
      x = data.vertexes_data.A[i][0] * scale;
      y = -data.vertexes_data.A[i][1] * scale;
    } else {
      x = (d * data.vertexes_data.A[i][0]) / (data.vertexes_data.A[i][2] + d) *
          scale;
      y = (d * data.vertexes_data.A[i][1]) / (data.vertexes_data.A[i][2] + d) *
          scale;
    }
    if (currentVertexShape == Round) {
      QGraphicsEllipseItem *ellipse =
          scene->addEllipse(x - vertexSize / 2, y - vertexSize / 2, vertexSize,
                            vertexSize, vertexPen);
      vertexItems.push_back(ellipse);
    } else if (currentVertexShape == Square) {
      QGraphicsRectItem *rect =
          scene->addRect(x - vertexSize / 2, y - vertexSize / 2, vertexSize,
                         vertexSize, vertexPen);
      vertexSquareItems.push_back(rect);
    } else if (currentVertexShape == Nan) {
    }
  }
}

void Viewer::on_btn_open_file_clicked() {
  QString fileName;
  fileName = QFileDialog::getOpenFileName(
      this, tr("Open OBJ"), "/home/d3/C8_3DViewer_v1.0-1/src/Obj",
      tr("OBJ Files (*.obj) ;; All files (*.*)"));

  if (!fileName.isEmpty()) {
    // Обнуляем структуру model_data перед загрузкой новых данных
    memset(&data, 0, sizeof(model_data));
    free_data(&data);
    QFileInfo fi(fileName);
    ui->lbl_file_name->setText(fi.fileName());
    char input_str[255] = {};
    qDebug() << fileName;
    get_str(input_str, fileName);
    qDebug() << input_str;

    if (!parse_obj(input_str, &data)) {
      QString status = "Vertex = " + QString::number(data.vertex_count) +
                       " Planes = " + QString::number(data.facets_count) +
                       " Path: " + fileName;
      ui->statusbar->showMessage(status);
      isModelExist = 1;
      drawLines();
      drawVertices();
      qDebug() << "";
    }
  }
}

void Viewer::on_zoom_slider_sliderReleased() {
  double slider = ui->zoom_slider->value();
  scale *= slider / 50.0;
  if (isModelExist) {
    drawLines();
    drawVertices();
  }
  qDebug() << slider << " slide " << scale;
  ui->zoom_slider->setValue(50);
}

void Viewer::on_btn_return_clicked() {
  scale = 50;
  drawLines();
  drawVertices();
}

void Viewer::on_btn_scale_clicked() {
  scale_model(&data, ui->spin_scale_x->value());
  if (isModelExist) {
    drawLines();
    drawVertices();
  }
}

void Viewer::on_btn_move_clicked() {
  move_model(&data, 'x', ui->spin_move_x->value());
  move_model(&data, 'y', ui->spin_move_y->value());
  move_model(&data, 'z', ui->spin_move_z->value());
  if (isModelExist) {
    drawLines();
    drawVertices();
  }
}

void Viewer::on_btn_rotate_clicked() {
  rotate_model(&data, 'x', ui->spin_rotate_x->value());
  rotate_model(&data, 'y', ui->spin_rotate_y->value());
  rotate_model(&data, 'z', ui->spin_rotate_z->value());
  if (isModelExist) {
    drawLines();
    drawVertices();
  }
}

void Viewer::on_btn_clear_rotate_clicked() {
  ui->spin_rotate_x->setValue(0);
  ui->spin_rotate_y->setValue(0);
  ui->spin_rotate_z->setValue(0);
}

void Viewer::on_btn_clear_move_clicked() {
  ui->spin_move_x->setValue(0);
  ui->spin_move_y->setValue(0);
  ui->spin_move_z->setValue(0);
}

void Viewer::on_btn_clear_scale_clicked() { ui->spin_scale_x->setValue(1); }

void Viewer::on_btn_jpeg_clicked() {
  QString save;
  save = QFileDialog::getSaveFileName(this, nullptr, nullptr,
                                      tr("JPEG (*.jpeg);;All Files (*)"));
  if (!save.isNull()) {
    QImage image(ui->Widget_3D->viewport()->rect().size(),
                 QImage::Format_ARGB32);
    QPainter painter(&image);
    ui->Widget_3D->scene()->render(&painter);
    image.save(save, "JPEG", 100);
  }
}

void Viewer::on_btn_bmp_clicked() {
  QString save;
  save = QFileDialog::getSaveFileName(this, nullptr, nullptr,
                                      tr("BMP (*.bmp);;All Files (*)"));
  if (!save.isNull()) {
    QImage image(ui->Widget_3D->viewport()->rect().size(),
                 QImage::Format_ARGB32);
    QPainter painter(&image);
    ui->Widget_3D->scene()->render(&painter);
    image.save(save, "BMP");
  }
}

void Viewer::on_btn_color_screen_clicked() {
  colorBack = colorDialog.getColor(colorBack, this, "Choose Background Color");
  if (colorBack.isValid()) {
    ui->Widget_3D->setStyleSheet(
        QString("background-color: %1").arg(colorBack.name()));
    ui->Widget_3D->scene()->setBackgroundBrush(colorBack);
    setButtonColor(ui->btn_color_screen, colorBack);
  }
}

void Viewer::on_btn_color_vershini_clicked() {
  QColor temp;
  temp = colorDialog.getColor(colorVertexes, this, "Choose Vertexes Color");
  if (temp.isValid()) {
    colorVertexes = temp;
  }
  setButtonColor(ui->btn_color_vershini, colorVertexes);
  vertexPen.setColor(colorVertexes);
  if (isModelExist) {
    drawLines();
    drawVertices();
  }
}

void Viewer::on_btn_color_line_clicked() {
  QColor temp;
  temp = colorDialog.getColor(pen_line.color(), this, "Choose Line Color");
  if (temp.isValid()) {
    colorLine = temp;
  }
  setButtonColor(ui->btn_color_line, colorLine);
  pen_line.setColor(colorLine);
  if (isModelExist) {
    drawLines();
    drawVertices();
  }
}

void Viewer::setButtonColor(QPushButton *button, QColor color) {
  QString colorString = QString("background-color: %1").arg(color.name());
  button->setStyleSheet(colorString);

  int brightness = color.red() * 299 + color.green() * 587 + color.blue() * 114;
  brightness /= 1000;
  QColor textColor;
  if (brightness < 128) {
    // Фон темный
    textColor = QColor(Qt::white);
  } else {
    // Фон светлый
    textColor = QColor(Qt::black);
  }
  button->setStyleSheet(QString("color: %1; background-color: %2;")
                            .arg(textColor.name(), color.name()));
}

void Viewer::on_btn_radio_parallel_clicked() {
  currentProjection = Parallel;
  if (isModelExist) {
    drawLines();
    drawVertices();
  }
}

void Viewer::on_btn_radio_central_clicked() {
  currentProjection = Central;
  if (isModelExist) {
    drawLines();
    drawVertices();
  }
}

void Viewer::on_btn_radio_sploshnaya_clicked() {
  pen_line.setStyle(Qt::SolidLine);
  if (isModelExist) {
    drawLines();
    drawVertices();
  }
}

void Viewer::on_btn_radio_punktir_clicked() {
  pen_line.setStyle(Qt::DashLine);
  if (isModelExist) {
    drawLines();
    drawVertices();
  }
}

void Viewer::on_spin_line_tolshina_valueChanged(double arg1) {
  lineWidth = arg1;
  if (isModelExist) {
    drawLines();
    drawVertices();
  }
}

void Viewer::on_spin_vershini_size_valueChanged(double arg1) {
  vertexSize = arg1;
  if (isModelExist) {
    drawLines();
    drawVertices();
  }
}

void Viewer::on_btn_radio_nan_clicked() {
  currentVertexShape = Nan;
  if (isModelExist) {
    drawLines();
    drawVertices();
  }
}

void Viewer::on_btn_radio_kryg_clicked() {
  currentVertexShape = Round;
  if (isModelExist) {
    drawLines();
    drawVertices();
  }
}

void Viewer::on_btn_radio_cube_clicked() {
  currentVertexShape = Square;
  if (isModelExist) {
    drawLines();
    drawVertices();
  }
}

void Viewer::gifScreencast() {
  qDebug() << "Start gif";
  isGifRecording = true;  // Запись начата

  QString exePath = QCoreApplication::applicationDirPath();
  QDir dir(exePath);
  auto folderPath = dir.filePath("screen");

  // Создание папки, если она еще не существует
  if (!dir.exists(folderPath)) {
    dir.mkpath(folderPath);
  }

  auto fileName = folderPath + "/gif_image.gif";
  int delay = 20;
  GifWriter g;
  // Создание файла и запись GIF
  GifBegin(&g, fileName.toStdString().c_str(), 640, 480, delay);

  QElapsedTimer timer;
  timer.start();
  QImage lastFrame;
  while (timer.elapsed() <= 5000) {
    QPixmap pix_map =
        ui->Widget_3D->grab();  // Получаем текущий кадр QGraphicsView
    QImage currentFrame =
        pix_map.toImage()
            .mirrored(false, true)
            .convertToFormat(QImage::Format_RGBA8888)
            .scaled(640, 480, Qt::IgnoreAspectRatio,
                    Qt::SmoothTransformation);  // Сначала отражаем, затем
                                                // конвертируем и масштабируем
                                                // до 640x480 с сглаживанием

    if (currentFrame != lastFrame) {
      lastFrame = currentFrame;
      std::vector<uint8_t> buffer(
          currentFrame.sizeInBytes());  // Буфер для байтов изображения
      memcpy(buffer.data(), currentFrame.constBits(),
             currentFrame.sizeInBytes());
      GifWriteFrame(&g, buffer.data(), 640, 480, delay);
    }

    QCoreApplication::processEvents();  // Обрабатываем события, чтобы
                                        // приложение оставалось отзывчивым
    std::this_thread::sleep_for(std::chrono::milliseconds(delay));
  }
  GifEnd(&g);
  qDebug() << "End gif";
  isGifRecording = false;  // Запись завершена
}

void Viewer::on_btn_gif_clicked() {
  if (isGifRecording) {
    qDebug() << "Another GIF recording task is running";
    // Не запускаем новый поток, если уже выполняется запись
  } else {
    std::thread cast_gif(&Viewer::gifScreencast, this);
    cast_gif.detach();
  }
}

void Viewer::scaleWithMouseWheel(QWheelEvent *event) {
  if (event->angleDelta().y() > 0) {
    scale *= 1.1;  // Увеличиваем масштаб при прокрутке вверх
  } else {
    scale /= 1.1;  // Уменьшаем масштаб при прокрутке вниз
  }
  drawLines();
  drawVertices();
}

bool Viewer::eventFilter(QObject *target, QEvent *event) {
  if (target == ui->Widget_3D->viewport() && event->type() == QEvent::Wheel) {
    QWheelEvent *wheelEvent = static_cast<QWheelEvent *>(event);
    scaleWithMouseWheel(wheelEvent);
    return true;  // event was handled
  }
  // standard event processing
  return QObject::eventFilter(target, event);
}

void Viewer::set_settings_from_data_to_window(settings_data *settings) {
  init_settings(settings);
  if (!strcmp(settings->projection_type, "parallel")) {
    ui->btn_radio_parallel->setChecked(true);
  } else if (!strcmp(settings->projection_type, "central")) {
    ui->btn_radio_central->setChecked(true);
  }
  if (!strcmp(settings->line_type, "solid")) {
    ui->btn_radio_sploshnaya->setChecked(true);
  } else if (!strcmp(settings->line_type, "dotted")) {
    ui->btn_radio_punktir->setChecked(true);
  }
  if (!strcmp(settings->line_type, "solid")) {
    ui->btn_radio_sploshnaya->setChecked(true);
  } else if (!strcmp(settings->line_type, "dotted")) {
    ui->btn_radio_punktir->setChecked(true);
  }
  if (!strcmp(settings->vertexes_paint_type, "none")) {
    ui->btn_radio_nan->setChecked(true);
  } else if (!strcmp(settings->vertexes_paint_type, "round")) {
    ui->btn_radio_kryg->setChecked(true);
  } else if (!strcmp(settings->vertexes_paint_type, "square")) {
    ui->btn_radio_cube->setChecked(true);
  }

  QString colorQString = QString::fromUtf8(settings->line_color);
  colorLine.setRed(colorQString.mid(1, 2).toInt(nullptr, 16));
  colorLine.setGreen(colorQString.mid(3, 2).toInt(nullptr, 16));
  colorLine.setBlue(colorQString.mid(5, 2).toInt(nullptr, 16));
  setButtonColor(ui->btn_color_line, colorLine);

  colorQString = QString::fromUtf8(settings->vertexes_color);
  colorVertexes.setRed(colorQString.mid(1, 2).toInt(nullptr, 16));
  colorVertexes.setGreen(colorQString.mid(3, 2).toInt(nullptr, 16));
  colorVertexes.setBlue(colorQString.mid(5, 2).toInt(nullptr, 16));
  setButtonColor(ui->btn_color_vershini, colorVertexes);

  colorQString = QString::fromUtf8(settings->background_color);
  colorBack.setRed(colorQString.mid(1, 2).toInt(nullptr, 16));
  colorBack.setGreen(colorQString.mid(3, 2).toInt(nullptr, 16));
  colorBack.setBlue(colorQString.mid(5, 2).toInt(nullptr, 16));
  ui->Widget_3D->setStyleSheet(
      QString("background-color: %1").arg(colorBack.name()));
  setButtonColor(ui->btn_color_screen, colorBack);

  ui->spin_line_tolshina->setValue(settings->line_width);
  ui->spin_vershini_size->setValue(settings->vertexes_size);
}

void Viewer::save_settings_from_window(settings_data *settings) {
  int status = 0;
  set_default_settings(settings);
  if (ui->btn_radio_parallel->isChecked()) {
    strcpy(settings->projection_type, "parallel");
  } else if (ui->btn_radio_central->isChecked()) {
    strcpy(settings->projection_type, "central");
  }
  if (ui->btn_radio_sploshnaya->isChecked()) {
    strcpy(settings->line_type, "solid");
  } else if (ui->btn_radio_punktir->isChecked()) {
    strcpy(settings->line_type, "dotted");
  }

  if (ui->btn_radio_nan->isChecked()) {
    strcpy(settings->vertexes_paint_type, "none");
  } else if (ui->btn_radio_kryg->isChecked()) {
    strcpy(settings->vertexes_paint_type, "round");
  } else if (ui->btn_radio_cube->isChecked()) {
    strcpy(settings->vertexes_paint_type, "square");
  }

  int red = colorLine.red();
  int green = colorLine.green();
  int blue = colorLine.blue();
  sprintf(settings->line_color, "#%02X%02X%02X", red, green, blue);

  red = colorVertexes.red();
  green = colorVertexes.green();
  blue = colorVertexes.blue();
  sprintf(settings->vertexes_color, "#%02X%02X%02X", red, green, blue);

  red = colorBack.red();
  green = colorBack.green();
  blue = colorBack.blue();
  sprintf(settings->background_color, "#%02X%02X%02X", red, green, blue);

  settings->line_width = ui->spin_line_tolshina->value();
  settings->vertexes_size = ui->spin_vershini_size->value();
  status = save_settings(settings);
  if (status) {
    QMessageBox::critical(
        nullptr, "Error",
        "Cannot write settings to file, exiting without saving!");
  }
}
