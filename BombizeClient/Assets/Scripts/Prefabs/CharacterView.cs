using System.Collections.Generic;
using DataStructures;
using UnityEngine;

public class CharacterView : MonoBehaviour
{
    [SerializeField] private List<SpriteRenderer> _left;
    [SerializeField] private List<SpriteRenderer> _up;
    [SerializeField] private List<SpriteRenderer> _down;
    private List<SpriteRenderer>[] _directions = new List<SpriteRenderer>[3];
    private Vector3 _flipX = new Vector3(-1f, 1f, 1f);

    private void Awake()
    {
        _directions[0] = _left;
        _directions[1] = _up;
        _directions[2] = _down;

        foreach (var dir in _directions)
            foreach(var sr in dir)
                sr.gameObject.SetActive(true);
                
        SetDirection(Direction.No);
    }

    public void SetDirection(Direction direction)
    {
        int indexSetTrue = 0;

        if (direction == Direction.No || direction == Direction.Down)
        {
            indexSetTrue = 2;
            if (transform.localScale.x < 0)
                transform.localScale = Vector3.Scale(transform.localScale, _flipX);
        }
        else if (direction == Direction.Right)
        {
            indexSetTrue = 0;
            if (transform.localScale.x > 0)
                transform.localScale = Vector3.Scale(transform.localScale, _flipX);
        }
        else    // Left or Up
        {
            indexSetTrue = (direction == Direction.Left) ? 0 : 1; 
            
            if (transform.localScale.x < 0)
                transform.localScale = Vector3.Scale(transform.localScale, _flipX);
        }

        foreach (var direct in _directions)
            direct.ForEach(sr => sr.enabled = false );

        _directions[indexSetTrue].ForEach(sr => sr.enabled = true );
    }
}
