#ifndef __CPPGEN_GNET_SKILL250
#define __CPPGEN_GNET_SKILL250
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill250:public Skill
    {
      public:
        enum
        { SKILL_ID = 250 };
          Skill250 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill250Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 1600;
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
                return 400;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetRatio (1 + 0.04 * skill->GetT3 ());
                int decfhp = 20;
                  skill->GetPlayer ()->SetHp (1 + norm (skill->GetPlayer ()->GetHp () / decfhp) * (skill->GetPlayer ()->GetHp () - decfhp));
                  skill->
                    SetPlus ((1 +
                              norm (skill->GetPlayer ()->GetHp () / decfhp) * (INT (1.42 - 0.05 * skill->GetLevel ()) *
                                                                               (0.058 * skill->GetLevel () * skill->GetLevel () +
                                                                                12 * skill->GetLevel () + 443) + INT (0.05 * skill->GetLevel () +
                                                                                                                      0.55) * (59 +
                                                                                                                               67 *
                                                                                                                               skill->GetLevel () -
                                                                                                                               0.14 *
                                                                                                                               skill->GetLevel () *
                                                                                                                               skill->GetLevel ()))) *
                             (1 + 0.08 * skill->GetT0 ()));
                  skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill250Stub ():SkillStub (250)
        {
            occupation = 2;
            name = L"·ÙĞÄÕ¶";
            nativename = "·ÙĞÄÕ¶";
            icon = "·ÙĞÄÕ¶.dds";
            maxlevel = 20;
            maxlearn = 9;
            type = 1;
            eventflag = 0;
            spcost = 1;
            timetype = 0;
            targettype = 0;
            autoattack = 1;
            allowform = 0;
            credittype = 0;
            serialskill = 0;
            charge = 0;
            chargedist = 0;
            succeedskillcolor = 0;
            succeedskillcolornum = 0;
            comboskill = 0;
            cycle = 0;
            cyclegfx = "";
            cyclemode = 0;
            skill_class = 0;
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
            preskillnum = 1;
            talent[0] = 630;
            talent[1] = 640;
            talent[2] = 644;
            talent[3] = 660;
            talent_size = 4;
            action[0] = "¹íÍõ×Ú_·ÙĞÄÕ¶";
            action[1] = "¹íÍõ×Ú_·ÙĞÄÕ¶";
            action[2] = "¹íÍõ×Ú_·ÙĞÄÕ¶";
            action[3] = "¹íÍõ×Ú_·ÙĞÄÕ¶";
            action[4] = "¹íÍõ×Ú_·ÙĞÄÕ¶";
            action[5] = "¹íÍõ×Ú_·ÙĞÄÕ¶";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "¹íÍõ×Ú_Æï³Ë_·ÙĞÄÕ¶";
            action[16] = "¹íÍõ×Ú_Æï³Ë_·ÙĞÄÕ¶";
            action[17] = "¹íÍõ×Ú_Æï³Ë_·ÙĞÄÕ¶";
            action[18] = "¹íÍõ×Ú_Æï³Ë_·ÙĞÄÕ¶";
            action[19] = "¹íÍõ×Ú_Æï³Ë_·ÙĞÄÕ¶";
            action[20] = "¹íÍõ×Ú_Æï³Ë_·ÙĞÄÕ¶";
            action[21] = "";
            action[22] = "";
            action[23] = "";
            action[24] = "";
            action[25] = "";
            action[26] = "";
            action[27] = "";
            action[28] = "";
            action[29] = "";
            action[30] = "¼¼ÄÜ¹¥»÷1";
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
            skill_limit = 0;
#ifdef _SKILL_SERVER
            statestub.push_back (new State1 ());
            statestub.push_back (new State2 ());
#endif
        }
        virtual ~ Skill250Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 258 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 5 };
            return array[index];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (12);
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
            return 2000;
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[9] = { 60, 62, 64, 66, 68, 70, 72, 74, 76 };
            return array[level - 1];
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (7);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (1.7);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               19,
                               INT (INT (1.42 - 0.05 * skill->GetLevel ()) *
                                    (0.058 * skill->GetLevel () * skill->GetLevel () + 12 * skill->GetLevel () + 443) +
                                    INT (0.05 * skill->GetLevel () + 0.55) * (59 + 67 * skill->GetLevel () -
                                                                              0.14 * skill->GetLevel () * skill->GetLevel ())));

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
            skill->GetVictim ()->SetProbability (1.0 * (15 + 6 * skill->GetT1 ()));
            skill->GetVictim ()->SetTime (6000 + 100 + 3000 * skill->GetT1 ());
            skill->GetVictim ()->SetRatio ((0.05 + 0.03 * skill->GetT2 ()) * (1 + INT (zrand (100) / (100 - 6 * skill->GetT2 ()))));
            skill->GetVictim ()->SetBuffid (3);
            skill->GetVictim ()->SetTiansha (1);
            return true;
        }
#endif
    };
}
#endif
