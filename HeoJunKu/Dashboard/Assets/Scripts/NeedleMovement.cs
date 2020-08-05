using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class NeedleMovement : MonoBehaviour
{

    RectTransform pannelImageRect;

    // Start is called before the first frame update
    void Start()
    {
        pannelImageRect = GameObject.Find("Velocity").GetComponent<RectTransform>();
    }

    // Update is called once per frame
    void Update()
    {
        this.transform.Rotate(new Vector3(0, 0, -10), Space.Self);
    }
}
