#ifndef __CPPGEN_GNET_SKILL3016
#define __CPPGEN_GNET_SKILL3016
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3016:public Skill
    {
      public:
        enum
        { SKILL_ID = 3016 };
          Skill3016 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3016Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 1350;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetPray (1);
            }
            bool Cancel (Skill * skill) const
            {
                return 1;
            }
        };
#endif
#ifdef _SKILL_SERVER
        class State2:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 650;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetVar1 (skill->GetPlayer ()->GetMaxatk ());
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill3016Stub ():SkillStub (3016)
        {
            occupation = 167;
            name = L"ÄæÌì½Ù";
            nativename = "ÄæÌì½Ù";
            icon = "ÈËÌåÕ¨µ¯.dds";
            maxlevel = 20;
            maxlearn = 20;
            type = 1;
            eventflag = 0;
            spcost = 0;
            timetype = 0;
            targettype = 0;
            autoattack = 0;
            allowform = 0;
            credittype = 0;
            clearmask = 1;
            serialskill = 0;
            charge = 0;
            chargedist = 0;
            succeedskillcolor = 1;
            succeedskillcolornum = 1;
            comboskill = 0;
            cycle = 0;
            cyclegfx = "";
            cyclemode = 0;
            skill_class = -2;
            guide = false;
            summon_id = 0;
            trigger = false;
            castInPreSkill = false;
            preCastSkillId = 0;
            darkLightType = 0;
            followskill = 0;
            targetcnt = 0;
            excludemonstercnt = 0;
            skillelemsnum = 0;
            skillelemsfactor = 0;
            preskillnum = 0;
            action[0] = "ÏÉ_±ùÉË";
            action[1] = "ÏÉ_±ùÉË";
            action[2] = "ÏÉ_±ùÉË";
            action[3] = "ÏÉ_±ùÉË";
            action[4] = "ÏÉ_±ùÉË";
            action[5] = "ÏÉ_±ùÉË";
            action[6] = "ÏÉ_±ùÉË";
            action[7] = "ÏÉ_±ùÉË";
            action[8] = "ÏÉ_±ùÉË";
            action[9] = "ÏÉ_±ùÉË";
            action[10] = "ÏÉ_±ùÉË";
            action[11] = "ÏÉ_±ùÉË";
            action[12] = "ÏÉ_±ùÉË";
            action[13] = "ÏÉ_±ùÉË";
            action[14] = "ÏÉ_±ùÉË";
            action[15] = "Æï³Ë_ÏÉ_±ùÉË";
            action[16] = "Æï³Ë_ÏÉ_±ùÉË";
            action[17] = "Æï³Ë_ÏÉ_±ùÉË";
            action[18] = "Æï³Ë_ÏÉ_±ùÉË";
            action[19] = "Æï³Ë_ÏÉ_±ùÉË";
            action[20] = "Æï³Ë_ÏÉ_±ùÉË";
            action[21] = "Æï³Ë_ÏÉ_±ùÉË";
            action[22] = "Æï³Ë_ÏÉ_±ùÉË";
            action[23] = "Æï³Ë_ÏÉ_±ùÉË";
            action[24] = "Æï³Ë_ÏÉ_±ùÉË";
            action[25] = "Æï³Ë_ÏÉ_±ùÉË";
            action[26] = "Æï³Ë_ÏÉ_±ùÉË";
            action[27] = "Æï³Ë_ÏÉ_±ùÉË";
            action[28] = "Æï³Ë_ÏÉ_±ùÉË";
            action[29] = "Æï³Ë_ÏÉ_±ùÉË";
            action[30] = "0";
            rangetype = 0;
            doenchant = true;
            dobless = false;
            need_item = false;
            item_relation = 0;
            item1_id = -1;
            item1_num = 0;
            item2_id = -1;
            item2_num = 0;
            weapon_limit = -1;
            skill_limit = 1;
#ifdef _SKILL_SERVER
            statestub.push_back (new State1 ());
            statestub.push_back (new State2 ());
#endif
        }
        virtual ~ Skill3016Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (190 + 50 * skill->GetLevel () + 4 * skill->GetLevel () * skill->GetLevel ());
        }
        int GetDivinityExp (int level) const
        {
            static int array[20] =
                { 400, 500, 500, 700, 1500, 3100, 3900, 6500, 10300, 16900, 21800, 43300, 49900, 81300, 84500, 197600, 340200, 435500, 998600,
 1897100 };
            return array[level - 1];
        }
        int GetDivinityLevel (int level) const
        {
            static int array[20] = { 2, 4, 6, 8, 10, 13, 16, 19, 23, 27, 31, 36, 41, 46, 51, 57, 63, 69, 75, 81 };
            return array[level - 1];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (10 + 0.5 * skill->GetLevel ());
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (0);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 2000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 300000 - 10000 * skill->GetLevel ();
        }
        int GetEnmity (Skill * skill) const
        {
            return 0;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[20] = { 120, 122, 124, 126, 128, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 141, 143, 145, 147, 150 };
            return array[level - 1];
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (5 + 0.5 * skill->GetLevel ());
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (2 + 0.5 * skill->GetLevel ());
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               190 + 50 * skill->GetLevel () + 4 * skill->GetLevel () * skill->GetLevel (),
                               300 - 10 * skill->GetLevel (),
                               2 + 0.5 * skill->GetLevel (),
                               INT (3 + INT (skill->GetLevel () / 3.9)),
                               0.8 + 0.2 * skill->GetLevel (), 0.8 + 0.4 * skill->GetLevel (), 5 + 0.5 * skill->GetLevel ());

        }
#endif
#ifdef _SKILL_CLIENT
        int GetIntroduction (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, skill->GetShowlevel ());

        }
#endif
#ifdef _SKILL_SERVER
        bool StateAttack (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetTime (10050);
            skill->GetVictim ()->SetRatio (0.2);
            skill->GetVictim ()->SetBuffid (3 + INT (skill->GetLevel () / 5));
            skill->GetVictim ()->SetAmount (15);
            skill->GetVictim ()->SetValue (zrand (105 + 0.5 * skill->GetLevel ()) >
                                           100 ? ((0.8 + 0.2 * skill->GetLevel () + zrand (skill->GetLevel ()) * 0.2) * 1.5) *
                                           skill->GetPlayer ()->GetVar1 () * (1 + norm (skill->GetLevel () / 19.9) * 0.1) : (0.8 +
                                                                                                                             0.2 *
                                                                                                                             skill->GetLevel () +
                                                                                                                             zrand (skill->
                                                                                                                                    GetLevel ()) *
                                                                                                                             0.2) *
                                           skill->GetPlayer ()->GetVar1 () * (1 + norm (skill->GetLevel () / 19.9) * 0.1));
            skill->GetVictim ()->SetHumanbomb (1);
            return true;
        }
#endif
    };
}
#endif
