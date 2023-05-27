using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;
using TMPro;

public class CoinsManagement : MonoBehaviour
{
    public TextMeshProUGUI scoreText;

    void Start()
    {
        Transform coinsTransform = transform.Find("Score");
        scoreText = coinsTransform.GetComponent<TextMeshProUGUI>();
        PlayerPrefs.SetInt("ScoreCount", 0);
    }

    void Update()
    {
        scoreText.text = PlayerPrefs.GetInt("ScoreCount", 0).ToString();
    }
}
