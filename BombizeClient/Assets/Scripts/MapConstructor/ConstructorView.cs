using System.IO;
using LibUIElements;
using UnityEngine;
using UnityEngine.UI;

namespace MapConstructor
{
    public class ConstructorView : MonoBehaviour
    {
        [SerializeField] private Constructor _mapConstructor;
        [SerializeField] private BrushesView _brushesView;
        [SerializeField] private PaletteView _backgroundsView;
        [SerializeField] private GridView _gridView;
        [SerializeField] private Image _currentPenImage;
        [SerializeField] private InputDialogView _saveMapDialog;
        private int _currentPenId;
        private const int DefaultPenId = 1;

        private void Awake()
        {
            _currentPenId = DefaultPenId;
            _currentPenImage.sprite = Locator.GameStorage.Instance.MapBlocks[_currentPenId].sprite;

            _backgroundsView.Init(
                (id) => _mapConstructor.SetBackground(id),
                Locator.GameStorage.Instance.MapBackgrounds
            );
        }

        public void SelectPen(int id)
        {
            _currentPenId = id;
            _mapConstructor.SetPen(_currentPenId);
            _currentPenImage.sprite = Locator.GameStorage.Instance.MapBlocks[id].sprite;
        }

        public void OnButtonCursor() => _mapConstructor.SetCursor();
        public void OnButtonPen() => _mapConstructor.SetPen(_currentPenId);
        public void OnButtonBrushes()
        {
            _currentPenId = DefaultPenId;
            _mapConstructor.SetCursor();
            _brushesView.Show();
        }
        public void OnButtonEraser() => _mapConstructor.SetEraser();
        public void OnButtonSpawner() => _mapConstructor.SetSpawner();
        public void OnButtonBackgrounds() => _backgroundsView.Show();
        public void OnButtonSwithGrid() => _gridView.Swith();
        public void OnButtonSave()
        {
            _saveMapDialog.Show("Сохранить в файл", "Имя для сохранения", (confirm, fileName) =>
            {
                if (confirm)
                {
                    if (fileName == "")
                        fileName = "TempMap.json";
                    else if (!fileName.EndsWith(".json", System.StringComparison.OrdinalIgnoreCase))
                        fileName += ".json";

                    _mapConstructor.Save(Path.Combine(Application.dataPath, "Maps", fileName)); // to Assets/Maps
                }
            });
        }

        public void OnButtonOpenFile()
        {
            _saveMapDialog.Show("Загрузить из файла", "Имя файла", (confirm, fileName) =>
            {
                if (confirm)
                {
                    if (fileName == "")
                        Debug.Log("The file name is not specified");
                    else if (!fileName.EndsWith(".json", System.StringComparison.OrdinalIgnoreCase))
                        fileName += ".json";

                    _mapConstructor.LoadFromFile(Path.Combine(Application.dataPath, "Maps", fileName)); // to Assets/Maps
                }
            });
        }
    }
}