using UnityEngine;
using DataStructures;

public class MovePlayer : MonoBehaviour
{
    public float Speed;
    private CharacterView _characterView;
    private Rigidbody2D _rigidbody2D;
    private Direction _direction;
    private Vector2 _dxVec2 = Vector2.zero;
    private Vector2 _dyVec2 = Vector2.zero;

    private void Start()
    {
        _characterView = GetComponent<CharacterView>();
        _rigidbody2D = GetComponent<Rigidbody2D>();
    }

    private void Update()
    {
        if (Input.GetKey(KeyCode.D))
        {
            _direction = Direction.Right;
        }
        else if (Input.GetKey(KeyCode.A))
        {
            _direction = Direction.Left;
        }
        else if (Input.GetKey(KeyCode.W))
        {
            _direction = Direction.Up;
        }
        else if (Input.GetKey(KeyCode.S))
        {
            _direction = Direction.Down;
        }
        else
        {
            _direction = Direction.No;
        }
    }

    private void FixedUpdate()
    {
        Vector2 nowPos = _rigidbody2D.position;
        Vector2 needPos = new Vector2(Mathf.Round(nowPos.x), Mathf.Round(nowPos.y));

        _dxVec2.x = (needPos.x - nowPos.x) * Speed;
        _dyVec2.y = (needPos.y - nowPos.y) * Speed;

        switch (_direction)
        {
            case Direction.Up:
                _rigidbody2D.velocity = Vector3.Normalize(Vector2.up + _dxVec2) * Speed;
                break;
            case Direction.Down:
                _rigidbody2D.velocity = Vector3.Normalize(Vector2.down + _dxVec2) * Speed;
                break;
            case Direction.Left:
                _rigidbody2D.velocity = Vector3.Normalize(Vector2.left + _dyVec2) * Speed;
                break;
            case Direction.Right:
                _rigidbody2D.velocity = Vector3.Normalize(Vector2.right + _dyVec2) * Speed;
                break;
            default:
                _rigidbody2D.velocity = Vector2.zero;
                break;
        }
        if (_direction != Direction.No) _characterView.SetDirection(_direction);
    }

}
