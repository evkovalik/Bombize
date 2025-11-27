using UnityEngine;

namespace Gameplay
{
    public class Bomb : MonoBehaviour
    {
        [SerializeField] private CircleCollider2D _collider;
        private void OnTriggerExit2D(Collider2D other)
        {
            _collider.isTrigger = false;
        }
    }
}