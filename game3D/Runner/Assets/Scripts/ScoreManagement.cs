using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;
using TMPro;

public class ScoreManagement : MonoBehaviour
{
    public TextMeshProUGUI scoreText;

    void Start()
    {
        Transform scoreTransform = transform.Find("Score");
        scoreText = scoreTransform.GetComponent<TextMeshProUGUI>();
        PlayerPrefs.SetInt("ScoreCount", 0);
        scoreText.text = PlayerPrefs.GetInt("ScoreCount", 0).ToString();
    }

    void Update()
    {
        scoreText.text = PlayerPrefs.GetInt("ScoreCount", 0).ToString();
    }
}
