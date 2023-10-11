#ifndef VIEWER_H  // Если VIEWER_H еще не определен
#define VIEWER_H  // Определяем VIEWER_H

// Необходимые библиотеки QT
#include <QBuffer>
#include <QColorDialog>  // Класс для выбора цвета
#include <QDialog>  // Класс для создания диалоговых окон
#include <QDockWidget>  // Виджет, который может быть "прикреплен" к основному окну l
#include <QElapsedTimer>  // Класс для измерения времени
#include <QFileDialog>  // Предоставляет класс для создания окна выбора файла
#include <QGraphicsEllipseItem>  // Эллипс
#include <QGraphicsScene>  // Базовый класс для управления большим количеством товаров графической области
#include <QImage>  // Класс для работы с растровыми изображениями
#include <QMainWindow>  // Основной класс для создания приложений на основе мндоконной архитектуры
#include <QMessageBox>  // Класс, предназначенный для создания окон сообщений
#include <QMovie>
#include <QPainter>     // Функционал рисования в Qt
#include <QPushButton>  // Класс для создания кнопок
#include <QWheelEvent>  // Содержит параметры для колесика мыши

// Включение сторонней C-библиотеки для просмотра 3D
extern "C" {
#include "../Cfiles/3d_viewer.h"
}

// Область имен для UI
QT_BEGIN_NAMESPACE
namespace Ui {
class Viewer;
}  // namespace Ui
QT_END_NAMESPACE

// Определение класса Viewer
class Viewer : public QMainWindow {
  Q_OBJECT  // Макрос, необходимый для функций QT

      public : Viewer(QWidget *parent = nullptr);  // Конструктор с опциональным
                                                   // родительским виджетом
  ~Viewer();  // Деструктор

  // Приватные слоты для обработки нажатий кнопок и других событий UI
 private slots:
  void on_btn_jpeg_clicked();  // Обработчик нажатия на кнопку JPEG (скорее
                               // всего для сохранения файла в формате JPEG).
  void on_btn_bmp_clicked();  // Обработчик нажатия на кнопку BMP (скорее всего
                              // для сохранения файла в формате BMP).
  void on_btn_color_screen_clicked();  // Обработчик нажатия на кнопку выбора
                                       // цвета экрана.
  void on_btn_color_vershini_clicked();  // Обработчик нажатия на кнопку выбора
                                         // цвета вершин.
  void on_btn_color_line_clicked();  // Обработчик нажатия на кнопку выбора
                                     // цвета линий.
  void on_btn_radio_parallel_clicked();  // Обработчик нажатия на radio-кнопку
                                         // выбора параллельной проекции.
  void on_btn_radio_central_clicked();  // Обработчик нажатия на radio-кнопку
                                        // выбора центральной проекции.
  void on_btn_radio_sploshnaya_clicked();  // Обработчик нажатия на radio-кнопку
                                           // выбора сплошной линии.
  void on_btn_radio_punktir_clicked();  // Обработчик нажатия на radio-кнопку
                                        // выбора пунктирной линии.
  void on_spin_line_tolshina_valueChanged(
      double arg1);  // Обработчик изменения значения спинеры толщины линий.
  void on_btn_radio_nan_clicked();  // Обработчик нажатия на radio-кнопку отмены
                                    // выбора форму вершин.
  void on_btn_radio_kryg_clicked();  // Обработчик нажатия на radio-кнопку
                                     // выбора формы вершин "круг".
  void on_btn_radio_cube_clicked();  // Обработчик нажатия на radio-кнопку
                                     // выбора формы вершин "куб".
  void on_spin_vershini_size_valueChanged(
      double arg1);  // Обработчик изменения значения спинеры размера вершин.
  void
  on_btn_open_file_clicked();  // Обработчик нажатия на кнопку открытия файла.
  void on_btn_rotate_clicked();  // Обработчик нажатия на кнопку начала вращения
                                 // модели.
  void
  on_btn_move_clicked();  // Обработчик нажатия на кнопку пермещения модели.
  void
  on_btn_return_clicked();  // Обработчик нажатия на кнопку возврата настроек.
  void on_btn_scale_clicked();  // Обработчик нажатия на кнопку масштабирования
                                // модели.
  void on_btn_clear_rotate_clicked();  // Обработчик нажатия на кнопку сброса
                                       // вращения.
  void on_btn_clear_move_clicked();  // Обработчик нажатия на кнопку сброса
                                     // перемещения.
  void on_btn_clear_scale_clicked();  // Обработчик нажатия на кнопку сброса
                                      // масштабирования.
  void on_zoom_slider_sliderReleased();  // Обработчик отпускания слайдера зума.
  void
  on_btn_gif_clicked();  // Обработчик нажатия на кнопку создания GIF-анимации.
  void scaleWithMouseWheel(
      QWheelEvent *event);  // Обработчик события прокрутки колесика мыши для
                            // масштабирования.

 private:
  // Настройки mainWindow и UI
  Ui::Viewer *ui;  // Объект интерфейса пользователя
  QColorDialog colorDialog;  // Диалог выбора цвета
  QGraphicsScene *scene;     // Объект сцены
  QColor colorLine = QColorConstants::White;      // Цвет линий
  QColor colorVertexes = QColorConstants::White;  // Цвет вершин
  QColor colorBack = QColorConstants::Gray;       // Цвет вершин

  // Настройки модели
  model_data data;  // Структура данных 3D модели
  settings_data settings;
  enum Projection { Parallel, Central };  // Перечисление типов проекций
  Projection currentProjection = Parallel;  // Текущий тип проекции
  enum VertexShape { Nan, Round, Square };  // Перечисление форм вершин
  VertexShape currentVertexShape = Round;  // Текущая форма вершин

  // Настройки карандашей и элеметов операторов
  QPen pen_line;   // Карандаш для линий
  QPen vertexPen;  // Карандаш для вершин
  QList<QGraphicsEllipseItem *>
      vertexItems;  // Список эллиптических элементов вершин
  QVector<QGraphicsRectItem *>
      vertexSquareItems;  // Вектор прямоугольников вершин

  // Настройки параметров отрисовки
  int max_W = 1000;  // Максимальная ширина отрисовки
  int max_H = 800;  // Максимальная высота зон обзора
  double vertexSize = 1.0;  // Размер вершины
  double lineWidth = 1.0;   // Ширина линии

  // Настройки координат
  double scale = 50;  // Масштабирование - коэффициент, который определяет
                      // степень увеличения или уменьшения представления.
  double
      Xmax = -1000000,
      Ymax = -1000000,
      Zmax =
          -1000000;  // Изначальные максимальные значения по каждой из
                     // координат, установлены в достаточно высокое значение для
                     // того, чтобы любая встреченная координата была больше.
  double x0,
      y0;  // Координаты начальной точки. Обычно используются для определения
           // начального положения в процессе перемещения или вращения.
  double x1, y1;  // Промежуточные координаты. Могут использоваться для хранения
                  // текущих координат в процессе перетаскивания, вращения или
                  // масштабирования.
  double x2,
      y2;  // Конечные координаты. Как правило, используются для обозначения
           // финального положения после перемещения или вращения.
  double x, y;  // Текущие координаты. Возможно, используются для трекинга
                // положения курсора или определенной точки на экране.
  double d = 500;  // Central projection distance - расстояние для центральной
                   // проекции. Это базовое значение, которое определяет, как
                   // объекты будут выглядеть при центральной проекции.
  QPointF lastMousePosition;  // Содержит последнее известное положение курсора
                              // мыши. Обычно используется для отслеживания
                              // движения мыши.
  QPointF currentMousePosition;  // Содержит текущее положение курсора мыши.
                                 // Используется для анализа движений мыши в
                                 // реальном времени.

  // Настройки состояния
  int isModelExist;
  int isGifRecording;  // Состояние записи GIF
  bool isMoveMode;     // Режим перемещения
  bool isRotateMode;   // Режим вращения

  // Методы отрисовки и вспомогательные функции
  void drawLines();     // Функция для отрисовки линий
  void drawVertices();  // Функция для отрисовки вершин
  void get_str(char *input_str,
               QString src);  // Вспомогательная функция для получения строки
  void setButtonColor(QPushButton *button,
                      QColor color);  // Функция для установки цвета кнопки
  void gifScreencast();  // Функция для создания GIF
  bool eventFilter(QObject *target,
                   QEvent *event);  // Функция для обработки событий
  void set_settings_from_data_to_window(settings_data *data);
  void save_settings_from_window(settings_data *data);
  void set_button_text_color(QPushButton *button,
                             const QColor &backgroundColor);
};
#endif  // VIEWER_H
