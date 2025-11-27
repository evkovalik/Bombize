using UnityEngine;

public static class Locator
{
    public static class GameStorage
    {
        private static Storage _instance;
        public static Storage Instance
        {
            get
            {
                if (_instance == null)
                {
                    _instance = Resources.Load<Storage>("Storage");
                    if (_instance == null)
                    {
                        Debug.LogError("Storage not found in Resources folder");
                    }
                }
                return _instance;
            }
        }
    }
}